// 示例：使用 convert_args 过程宏
// 这个文件展示了如何使用过程宏将函数参数转换为 ArgumentList

use crate::args::ArgumentList;
use bmnet_macros::convert_args;

// 使用宏前：fn test(arg0: i32, arg1: String) { ... }
// 使用宏后：fn test(args: ArgumentList) { ... }
// 
// 注意：在函数定义时，你应该写原始的参数列表（如 arg0: i32, arg1: String）
// 宏会自动将其转换为 args: ArgumentList，并在函数体开始处提取参数
#[convert_args]
pub fn test(arg0: i32, arg1: String) {
    // 函数体中可以直接使用 arg0 和 arg1，宏会自动从 ArgumentList 中提取
    println!("arg0: {}, arg1: {}", arg0, arg1);
}

// 另一个示例：只有 i32 参数
#[convert_args]
pub fn example_function(x: i32, y: i32) {
    println!("x: {}, y: {}", x, y);
}

// 有返回值的示例：返回值会通过 FnCallback 回调返回
// 使用宏前：fn add(x: i32, y: i32) -> i32 { x + y }
// 使用宏后：fn add(args: ArgumentList, callback: FnCallback) { ... }
#[convert_args]
pub fn add(x: i32, y: i32) -> i32 {
    x + y
}

// 返回 String 的示例
#[convert_args]
pub fn greet(name: String) -> String {
    format!("Hello, {}!", name)
}

// 使用 convert_args 宏实现 HTTP GET 请求
// 参数：url (String, 必需), headers (String, 可选，JSON 格式字符串), ext (usize, 扩展参数)
// 返回值：(String, usize) 元组 (通过 FnCallback 返回)
// 宏会自动添加 #[no_mangle] 和 extern "C"，使其可以被 C 语言调用
#[convert_args]
pub fn bmnet_get_ext_impl(url: String, headers: String, ext: usize) -> (String, usize) {
    use crate::web::client::get_url;
    use serde_json::{Map, Value};
    
    // 将 headers 字符串转换为 Option<Map<String, Value>>
    let header_map = if headers.is_empty() {
        None
    } else {
        match serde_json::from_str::<Value>(&headers) {
            Ok(json) => {
                if let Some(obj) = json.as_object() {
                    let mut map = Map::new();
                    for (k, v) in obj {
                        map.insert(k.clone(), v.clone());
                    }
                    Some(map)
                } else {
                    None
                }
            }
            Err(e) => {
                println!("bmnet_get_ext_impl: failed to parse headers JSON: {}", e);
                None
            }
        }
    };
    
    // 执行 GET 请求
    match get_url(url, header_map) {
        Ok(text) => {
            println!("http get_ext: {:?}", text);
            (text, ext)
        }
        Err(e) => {
            println!("http get_ext error: {:?}", e);
            (format!("Error: {}", e), ext)
        }
    }
}

