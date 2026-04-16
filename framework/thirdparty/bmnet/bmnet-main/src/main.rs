mod web;
mod args;
mod example;

// 测试程序
use web::client::{get_url};
use std::ffi::{CString, c_int};
use args::{ArgumentList, Argument, ArgumentValue, ArgumentString, FnCallback};
use example::{test, example_function, add, greet, bmnet_get_ext_impl};
//use tokio_core::reactor::Core; 

// https://httpbin.org/ip
// fn main() {
//     // Send GET request and inspect result, with proper error handling.
//     if let Err(e) = get(String::from("https://127.0.0.1:8443/user/v1/testlogin")) {
//         eprintln!("FAILED: {}", e);
//         std::process::exit(1);
//     }
// }
type Callback = unsafe extern "C" fn(data: *mut u8, len: usize) -> c_int;

type i32_ptr = *const i32;

struct A;

impl Drop for A {
    fn drop(&mut self) {
        println!("drop self");
    }
}

// struct Foo<T: ?Sized> {
//     a: Box<T>,
//     b: Box<T>,
// }

struct Foo<T> where T: ?Sized {
    a: Box<T>,
    b: Box<T>,
}

struct Te {
    t: Foo<dyn Fn(i32)>,
}

struct N {
    a: i32,
}

fn main() {
    // Send GET request and inspect result, with proper error handling.
    // if let Err(e) = get(String::from("https://127.0.0.1:8443/user/v1/testlogin")) {
    //     eprintln!("FAILED: {}", e);
    //     std::process::exit(1);
    // }

    let a: i32 = 10;
    // 无需加unsafe
    let b: *const i32 = &a;

    let n = Box::new(N { a: 100 });

    let n_p: *mut N = Box::into_raw(n);
    // 必须加unsafe
    unsafe {
        println!("main b: {}", *b);

        let p: usize = b as usize;
        // usize转原始指针
        let raw_p = p as i32_ptr;

        println!("main raw_p: {:p}", raw_p);

        // 转成c size_t
        let n_ext  = n_p as usize;
        // c size_t转为box
        let n_new = Box::from_raw(n_ext as *mut N);
        println!("main n_new: {}", n_new.a);
    }

    let result = get_url(String::from("https://127.0.0.1:8443/user/v1/testlogin"), None);
    match result {
        Ok(bytes) => println!("http get: {} bytes", bytes.len()),
        Err(e) => println!("http error: {:?}", e),
    }

    let t = Box::new(A{});

    let bt = Box::into_raw(t);

    // 测试 bmnet-macros 过程宏
    println!("\n=== 测试 bmnet-macros 过程宏 ===");
    test_macro_convert_args();
    
    // 测试 bmnet_get_ext_impl
    println!("\n=== 测试 bmnet_get_ext_impl ===");
    test_bmnet_get_ext_impl();

    println!("main end");
}

// 测试 convert_args 宏
fn test_macro_convert_args() {
    use std::os::raw::c_char;
    
    // 创建测试参数：arg0: i32 = 42, arg1: String = "Hello"
    let arg0_value : i32 = 42;
    let arg1_str = CString::new("Hello").unwrap();
    let arg1_ptr = arg1_str.as_ptr();
    
    // 创建 Argument 数组
    let mut args_vec = vec![
        Argument {
            arg_type: 1, // i32 类型
            arg_value: unsafe { ArgumentValue { i32_value: arg0_value } },
        },
        Argument {
            arg_type: 2, // String 类型
            arg_value: unsafe {
                ArgumentValue {
                    str: ArgumentString {
                        str_value: arg1_ptr,
                        str_len: arg1_str.as_bytes().len(),
                    },
                }
            },
        },
    ];
    
    // 创建 ArgumentList
    let arg_list = ArgumentList {
        arg_count: 2,
        arg_list: args_vec.as_mut_ptr(),
    };
    
    // 测试 test 函数（arg0: i32, arg1: String）
    println!("调用 test(arg0: i32, arg1: String):");
    test(arg_list);
    
    // 创建第二个测试：example_function(x: i32, y: i32)
    let mut args_vec2 = vec![
        Argument {
            arg_type: 1, // i32 类型
            arg_value: unsafe { ArgumentValue { i32_value: 10 } },
        },
        Argument {
            arg_type: 1, // i32 类型
            arg_value: unsafe { ArgumentValue { i32_value: 20 } },
        },
    ];
    
    let arg_list2 = ArgumentList {
        arg_count: 2,
        arg_list: args_vec2.as_mut_ptr(),
    };
    
    println!("\n调用 example_function(x: i32, y: i32):");
    example_function(arg_list2);
    
    // 防止 args_vec 和 arg1_str 被提前释放
    std::mem::forget(args_vec);
    std::mem::forget(args_vec2);
    std::mem::forget(arg1_str);
    
    println!("=== 宏测试完成 ===\n");
}

// 测试 bmnet_get_ext_impl 函数
fn test_bmnet_get_ext_impl() {
    use std::ffi::CString;
    
    // 定义 callback 函数来接收返回值
    unsafe extern "C" fn get_result_callback(result_args: ArgumentList) {
        println!("bmnet_get_ext_impl Callback 收到返回值，参数数量: {}", result_args.arg_count);
        
        if result_args.arg_count >= 2 {
            let result_ptr = result_args.arg_list;
            if !result_ptr.is_null() {
                unsafe {
                    // 第一个返回值：String
                    let arg0 = &*result_ptr;
                    match arg0.arg_type {
                        2 => {
                            // String 类型
                            let s = arg0.arg_value.str;
                            let str_value = unsafe { s.to_string_lossy() };
                            if str_value.is_empty() {
                                println!("返回值[0] 类型: String, 值: (空)");
                            } else {
                                println!("返回值[0] 类型: String, 长度: {} 字符", str_value.len());
                                // 只打印前300个字符，避免输出过长
                                if str_value.len() > 300 {
                                    println!("返回值[0] 内容（前300字符）: {}...", &str_value[..300]);
                                } else {
                                    println!("返回值[0] 内容: {}", str_value);
                                }
                            }
                        }
                        _ => {
                            println!("返回值[0] 未知类型: {}", arg0.arg_type);
                        }
                    }
                    
                    // 第二个返回值：usize
                    let arg1_ptr = result_ptr.add(1);
                    let arg1 = &*arg1_ptr;
                    match arg1.arg_type {
                        3 => {
                            // usize 类型
                            let ext_value = arg1.arg_value.usize_value;
                            println!("返回值[1] 类型: usize, 值: {}", ext_value);
                        }
                        _ => {
                            println!("返回值[1] 未知类型: {}", arg1.arg_type);
                        }
                    }
                }
            }
        } else {
            println!("返回值参数数量不足，期望至少 2 个，实际: {}", result_args.arg_count);
        }
    }
    
    // 测试 1: URL + 空 headers + ext
    println!("\n--- 测试 1: bmnet_get_ext_impl (URL + 空 headers + ext) ---");
    let url_str = CString::new("https://httpbin.org/get").unwrap();
    let url_ptr = url_str.as_ptr();
    
    let empty_headers_str = CString::new("").unwrap();
    let empty_headers_ptr = empty_headers_str.as_ptr();
    
    let ext_value: usize = 12345;
    
    let mut args_vec1 = vec![
        Argument {
            arg_type: 2, // String 类型 - URL
            arg_value: unsafe {
                ArgumentValue {
                    str: ArgumentString {
                        str_value: url_ptr,
                        str_len: url_str.as_bytes().len(),
                    },
                }
            },
        },
        Argument {
            arg_type: 2, // String 类型 - Headers (空)
            arg_value: unsafe {
                ArgumentValue {
                    str: ArgumentString {
                        str_value: empty_headers_ptr,
                        str_len: empty_headers_str.as_bytes().len(),
                    },
                }
            },
        },
        Argument {
            arg_type: 3, // usize 类型 - ext
            arg_value: unsafe { ArgumentValue { usize_value: ext_value } },
        },
    ];
    
    let arg_list1 = ArgumentList {
        arg_count: 3,
        arg_list: args_vec1.as_mut_ptr(),
    };
    
    println!("调用 bmnet_get_ext_impl(url: \"https://httpbin.org/get\", headers: \"\", ext: {})", ext_value);
    bmnet_get_ext_impl(arg_list1, get_result_callback);
    
    // 防止 args_vec1 和相关字符串被提前释放
    std::mem::forget(args_vec1);
    std::mem::forget(url_str);
    std::mem::forget(empty_headers_str);
    
    // 等待一下，让异步请求完成
    std::thread::sleep(std::time::Duration::from_millis(2000));
    
    // 测试 2: URL + headers + ext
    println!("\n--- 测试 2: bmnet_get_ext_impl (URL + headers + ext) ---");
    let url_str2 = CString::new("https://httpbin.org/get").unwrap();
    let url_ptr2 = url_str2.as_ptr();
    
    // 创建 headers JSON 字符串
    let headers_json = r#"{"User-Agent": "bmnet-test", "Accept": "application/json"}"#;
    let headers_str = CString::new(headers_json).unwrap();
    let headers_ptr = headers_str.as_ptr();
    
    let ext_value2: usize = 67890;
    
    let mut args_vec2 = vec![
        Argument {
            arg_type: 2, // String 类型 - URL
            arg_value: unsafe {
                ArgumentValue {
                    str: ArgumentString {
                        str_value: url_ptr2,
                        str_len: url_str2.as_bytes().len(),
                    },
                }
            },
        },
        Argument {
            arg_type: 2, // String 类型 - Headers
            arg_value: unsafe {
                ArgumentValue {
                    str: ArgumentString {
                        str_value: headers_ptr,
                        str_len: headers_str.as_bytes().len(),
                    },
                }
            },
        },
        Argument {
            arg_type: 3, // usize 类型 - ext
            arg_value: unsafe { ArgumentValue { usize_value: ext_value2 } },
        },
    ];
    
    let arg_list2 = ArgumentList {
        arg_count: 3,
        arg_list: args_vec2.as_mut_ptr(),
    };
    
    println!("调用 bmnet_get_ext_impl(url: \"https://httpbin.org/get\", headers: {}, ext: {})", headers_json, ext_value2);
    bmnet_get_ext_impl(arg_list2, get_result_callback);
    
    // 防止 args_vec2 和相关字符串被提前释放
    std::mem::forget(args_vec2);
    std::mem::forget(url_str2);
    std::mem::forget(headers_str);
    
    // 等待一下，让异步请求完成
    std::thread::sleep(std::time::Duration::from_millis(2000));
    
    println!("\n=== bmnet_get_ext_impl 测试完成 ===\n");
}


