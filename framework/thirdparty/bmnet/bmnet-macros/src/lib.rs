mod r#type;

use proc_macro::TokenStream;
use quote::quote;
use syn::{parse_macro_input, ItemFn, FnArg, Pat, Type, ReturnType, TypePath};
use r#type::{ARG_TYPE_I32, ARG_TYPE_STRING, ARG_TYPE_USIZE};

/// 过程宏：将函数签名从多个参数转换为单个 ArgumentList 参数
/// 如果函数有返回值，返回值将通过 FnCallback 回调返回
/// 
/// **重要**：宏会自动添加 `#[no_mangle]` 和 `extern "C"` 属性，使函数可以被 C 语言调用。
/// 如果函数已经包含这些属性，宏会保留它们。
/// 
/// 示例（无返回值）：
/// ```rust
/// #[convert_args]
/// pub fn test(arg0: i32, arg1: String) {
///     println!("arg0: {}, arg1: {}", arg0, arg1);
/// }
/// ```
/// 
/// 转换为：
/// ```rust
/// #[no_mangle]
/// pub extern "C" fn test(args: ArgumentList) {
///     let arg0: i32 = /* 从 args 提取 */;
///     let arg1: String = /* 从 args 提取 */;
///     println!("arg0: {}, arg1: {}", arg0, arg1);
/// }
/// ```
/// 
/// 示例（有返回值）：
/// ```rust
/// #[convert_args]
/// pub fn add(x: i32, y: i32) -> i32 {
///     x + y
/// }
/// ```
/// 
/// 转换为：
/// ```rust
/// #[no_mangle]
/// pub extern "C" fn add(args: ArgumentList, callback: FnCallback) {
///     let x: i32 = /* 从 args 提取 */;
///     let y: i32 = /* 从 args 提取 */;
///     let result: i32 = x + y;
///     // 将 result 包装成 ArgumentList 并通过 callback 返回
///     callback(return_arg_list);
/// }
/// ```
#[proc_macro_attribute]
pub fn convert_args(_attr: TokenStream, item: TokenStream) -> TokenStream {
    let input = parse_macro_input!(item as ItemFn);
    
    // 获取函数名、可见性、属性等
    let vis = &input.vis;
    let sig = &input.sig;
    let fn_name = &sig.ident;
    let fn_attrs = &input.attrs;
    let fn_block = &input.block;
    
    // 检查是否已经有 extern "C" 调用约定
    let has_extern_c = if let Some(abi) = &sig.abi {
        // 检查 ABI 名称是否为 "C"
        abi.name.as_ref().map(|lit| lit.value() == "C").unwrap_or(false)
    } else {
        false
    };
    
    // 检查是否已经有 #[no_mangle] 属性
    let has_no_mangle = fn_attrs.iter().any(|attr| {
        attr.path().is_ident("no_mangle")
    });
    
    // 提取参数名和类型（跳过 self 参数）
    let params: Vec<_> = sig
        .inputs
        .iter()
        .filter_map(|param| match param {
            FnArg::Receiver(_) => None,
            FnArg::Typed(pat_type) => {
                if let Pat::Ident(ident) = &*pat_type.pat {
                    Some((&ident.ident, &pat_type.ty))
                } else {
                    None
                }
            }
        })
        .collect();
    
    // 如果没有参数，直接返回原函数
    if params.is_empty() {
        return quote! {
            #(#fn_attrs)*
            #vis #sig #fn_block
        }
        .into();
    }
    
    // 检查是否有返回值
    let has_return = matches!(&sig.output, ReturnType::Type(_, _));
    
    // 生成新的函数签名参数
    // 如果有返回值，添加 callback: FnCallback 参数
    let new_params = if has_return {
        quote! { args: ArgumentList, callback: crate::args::FnCallback }
    } else {
        quote! { args: ArgumentList }
    };
    
    // 生成新的返回类型：转换后的函数总是返回 ()
    let return_type = quote! {};
    
    // 生成从 ArgumentList 提取参数的代码
    let arg_extractions: Vec<_> = params
        .iter()
        .enumerate()
        .map(|(idx, (name, type_ty))| {
            let extraction = generate_arg_extraction(idx, type_ty);
            quote! {
                let #name: #type_ty = #extraction;
            }
        })
        .collect();
    
    // 如果有返回值，需要包装函数体，将返回值通过 callback 返回
    let wrapped_block = if has_return {
        if let ReturnType::Type(_, return_ty) = &sig.output {
            let return_wrapper = generate_return_wrapper(return_ty);
            quote! {
                // 从 ArgumentList 中提取参数
                #(#arg_extractions)*
                
                // 执行原始函数体并获取返回值
                let result: #return_ty = {
                    #fn_block
                };
                
                // 将返回值包装成 ArgumentList 并通过 callback 返回
                #return_wrapper
            }
        } else {
            quote! {
                // 从 ArgumentList 中提取参数
                #(#arg_extractions)*
                
                // 原始函数体
                #fn_block
            }
        }
    } else {
        quote! {
            // 从 ArgumentList 中提取参数
            #(#arg_extractions)*
            
            // 原始函数体
            #fn_block
        }
    };
    
    // 生成函数属性：保留原有属性，并确保有 #[no_mangle]
    let no_mangle_attr = if !has_no_mangle {
        Some(quote! { #[no_mangle] })
    } else {
        None
    };
    
    // 生成新的函数签名，确保使用 extern "C" 调用约定
    // 如果已经有 extern "C"，保留原有的 ABI；否则添加 extern "C"
    let extern_abi = if !has_extern_c {
        // 添加 extern "C"
        quote! { extern "C" }
    } else {
        // 保留原有的 ABI（如果有的话）
        if let Some(abi) = &sig.abi {
            quote! { #abi }
        } else {
            quote! {}
        }
    };
    
    // 生成新的函数
    let output = quote! {
        #(#fn_attrs)*
        #no_mangle_attr
        #vis #extern_abi fn #fn_name(#new_params) #return_type {
            #wrapped_block
        }
    };
    
    output.into()
}

/// 获取类型的标识符名称（用于类型匹配）
fn get_type_ident(ty: &Type) -> Option<&syn::Ident> {
    if let Type::Path(TypePath { path, .. }) = ty {
        path.segments.last().map(|segment| &segment.ident)
    } else {
        None
    }
}

/// 为单个类型生成 Argument 包装代码
fn generate_single_arg_wrapper(value_expr: proc_macro2::TokenStream, type_ty: &Type) -> proc_macro2::TokenStream {
    match get_type_ident(type_ty) {
        Some(ident) if ident == "i32" => {
            quote! {
                Argument {
                    arg_type: #ARG_TYPE_I32,
                    arg_value: unsafe { ArgumentValue { i32_value: #value_expr } },
                }
            }
        }
        Some(ident) if ident == "usize" => {
            quote! {
                Argument {
                    arg_type: #ARG_TYPE_USIZE,
                    arg_value: unsafe { ArgumentValue { usize_value: #value_expr } },
                }
            }
        }
        Some(ident) if ident == "String" => {
            quote! {
                {
                    // 将 String 转换为 CString，并分配到堆上
                    let c_string = Box::new(CString::new(#value_expr).unwrap_or_else(|_| CString::new("").unwrap()));
                    let str_len = c_string.as_bytes().len();
                    let c_str_ptr = c_string.as_ptr();

                    // 防止 c_string 被提前释放（callback 可能异步使用）
                    std::mem::forget(c_string);

                    Argument {
                        arg_type: #ARG_TYPE_STRING,
                        arg_value: unsafe {
                            ArgumentValue {
                                str: crate::args::ArgumentString {
                                    str_value: c_str_ptr,
                                    str_len,
                                },
                            }
                        },
                    }
                }
            }
        }
        _ => {
            quote! {
                panic!(
                    "Unsupported return type element: {}. Supported types are: i32, usize, String",
                    stringify!(#type_ty)
                )
            }
        }
    }
}

/// 生成返回值包装代码，将返回值包装成 ArgumentList 并通过 callback 返回
/// 支持单个类型和元组类型
fn generate_return_wrapper(return_ty: &Type) -> proc_macro2::TokenStream {
    // 检查是否是元组类型
    if let Type::Tuple(tuple_ty) = return_ty {
        let elems = &tuple_ty.elems;
        let elem_count = elems.len();
        
        if elem_count == 0 {
            return quote! {
                panic!("Empty tuple return type is not supported");
            };
        }
        
        // 生成元组解构代码
        let mut tuple_fields = Vec::new();
        for i in 0..elem_count {
            let field_name = syn::Ident::new(&format!("elem_{}", i), proc_macro2::Span::call_site());
            tuple_fields.push(quote! { #field_name });
        }
        let tuple_pattern = quote! { (#(#tuple_fields),*) };
        
        // 为每个元素生成包装代码
        let mut arg_wrappers = Vec::new();
        
        for (i, elem_ty) in elems.iter().enumerate() {
            let field_name = syn::Ident::new(&format!("elem_{}", i), proc_macro2::Span::call_site());
            let arg_wrapper = generate_single_arg_wrapper(quote! { #field_name }, elem_ty);
            arg_wrappers.push(arg_wrapper);
        }
        
        return quote! {
            {
                use std::os::raw::c_char;
                use std::ffi::CString;
                use crate::args::{ArgumentList, Argument, ArgumentValue};
                
                // 解构元组
                let #tuple_pattern = result;
                
                // 创建返回值数组
                let mut return_args_vec = vec![
                    #(#arg_wrappers),*
                ];
                
                // 创建 ArgumentList
                let return_arg_list = ArgumentList {
                    arg_count: #elem_count as i32,
                    arg_list: return_args_vec.as_mut_ptr(),
                };
                
                // 通过 callback 返回
                unsafe {
                    callback(return_arg_list);
                }
                
                // 防止 return_args_vec 被提前释放（callback 可能异步使用）
                std::mem::forget(return_args_vec);
            }
        };
    }
    
    // 处理单个返回值类型
    let arg_wrapper = generate_single_arg_wrapper(quote! { result }, return_ty);
    
    quote! {
        {
            use std::os::raw::c_char;
            use std::ffi::CString;
            use crate::args::{ArgumentList, Argument, ArgumentValue};
            
            // 将返回值包装成 Argument
            let mut return_arg = Box::new(#arg_wrapper);
            
            // 创建 ArgumentList
            let return_arg_list = ArgumentList {
                arg_count: 1,
                arg_list: return_arg.as_mut() as *mut Argument,
            };
            
            // 通过 callback 返回
            unsafe {
                callback(return_arg_list);
            }
            
            // 防止 return_arg 被提前释放（callback 可能异步使用）
            std::mem::forget(return_arg);
        }
    }
}

/// 生成参数提取代码
fn generate_arg_extraction(idx: usize, type_ty: &Type) -> proc_macro2::TokenStream {
    let idx_plus_one = idx + 1;
    
    // 生成边界检查和获取参数指针的公共代码
    let bounds_check = quote! {
        if args.arg_count <= #idx as i32 {
            panic!(
                "Not enough arguments for function: expected at least {} arguments, got {}",
                #idx_plus_one,
                args.arg_count
            );
        }
        let arg_ptr = args.arg_list.add(#idx);
    };
    
    match get_type_ident(type_ty) {
        Some(ident) if ident == "i32" => {
            quote! {
                unsafe {
                    #bounds_check
                    if (*arg_ptr).arg_type != #ARG_TYPE_I32 {
                        panic!(
                            "Type mismatch at argument index {}: expected i32 (arg_type {}), got type {}",
                            #idx,
                            #ARG_TYPE_I32,
                            (*arg_ptr).arg_type
                        );
                    }
                    (*arg_ptr).arg_value.i32_value
                }
            }
        }
        Some(ident) if ident == "String" => {
            quote! {
                unsafe {
                    #bounds_check
                    if (*arg_ptr).arg_type != #ARG_TYPE_STRING {
                        panic!(
                            "Type mismatch at argument index {}: expected String (arg_type {}), got type {}",
                            #idx,
                            #ARG_TYPE_STRING,
                            (*arg_ptr).arg_type
                        );
                    }
                    let s = (*arg_ptr).arg_value.str;
                    crate::args::ArgumentString::to_string_lossy(&s)
                }
            }
        }
        Some(ident) if ident == "usize" => {
            quote! {
                unsafe {
                    #bounds_check
                    if (*arg_ptr).arg_type != #ARG_TYPE_USIZE {
                        panic!(
                            "Type mismatch at argument index {}: expected usize (arg_type {}), got type {}",
                            #idx,
                            #ARG_TYPE_USIZE,
                            (*arg_ptr).arg_type
                        );
                    }
                    (*arg_ptr).arg_value.usize_value
                }
            }
        }
        _ => {
            quote! {
                panic!(
                    "Unsupported argument type at index {}: {}. Supported types are: i32, String, usize",
                    #idx,
                    stringify!(#type_ty)
                );
            }
        }
    }
}

