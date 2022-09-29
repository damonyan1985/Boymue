mod web;

use web::client::{get_url};

use std::ffi::{CString, CStr, c_int};
use std::os::raw::{c_char};

type Callback = unsafe extern "C" fn(data: *const u8, len: usize);

fn exe_string_callback(cb: Option<Callback>, text: String) {
    if text.is_empty() {
        return
    }

    match cb {
        Some(callback) => { // 从option中解出callback指针
            // 转为[u8]数组切片
            let bytes = (&text).as_bytes();
            // 获取指针地址
            unsafe { callback(bytes.as_ptr(), bytes.len()); }  
        },
        None => {
            println!("callback is null")
        }
    }
}

// http/https get请求
#[no_mangle]
pub extern "C" fn bmnet_get(url: *const c_char, cb: Option<Callback>) {
    let c_url = unsafe { CStr::from_ptr(url) };

    let str_url = match c_url.to_str() {
        Ok(text) => text,
        Err(e) => "",
    };

    // url为空，返回
    if str_url.is_empty() {
        return
    }

    let result = get_url(String::from(str_url));
    match result {
        Ok(text) => {
            println!("http get: {:?}", text);
            exe_string_callback(cb, text);
        },
        Err(e) => println!("http error: {:?}", e),
    }

    println!("main get");
}