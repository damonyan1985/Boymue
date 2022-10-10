mod web;

use web::client::{get_url, post_url};

use std::ffi::{CString, CStr, c_int};
use std::os::raw::{c_char};
// extern crate rustc_serialize;
// use rustc_serialize::json;
use std::collections::HashMap;
use serde_json;
use serde_json::{Map, Value};
use tokio::runtime::Runtime;
use bmnet_timer::{Timer};

static mut HAS_TOKIO_RUNTIME: bool = false;

// 与c交互可能涉及会到一些unsafe操作
// ext扩展信息，如传递c++指针回调回去
type Callback = unsafe extern "C" fn(data: *const u8, len: usize, ext: usize);

fn exec_string_callback(cb: Option<Callback>, text: String, ext: usize) {
    if text.is_empty() {
        return
    }

    match cb {
        Some(callback) => { // 从option中解出callback指针
            // 转为[u8]数组切片
            let bytes = (&text).as_bytes();
            // 获取指针地址
            unsafe { callback(bytes.as_ptr(), bytes.len(), ext); }  
        },
        None => {
            println!("callback is null")
        }
    }
}

fn get_header(headers: *const c_char) -> Option<Map<String, Value>> {
    let mut header_map: Option<Map<String, Value>> = None;
    // 如果header字串不为空
    if !headers.is_null() {
        let c_headers = unsafe { CStr::from_ptr(headers) };

        let json_text = match c_headers.to_str() {
            Ok(text) => text,
            Err(e) => "",
        };

        if !json_text.is_empty() {
            let json: Value = serde_json::from_str(json_text).unwrap();
            header_map = Some(json.as_object().unwrap().clone());
            //println!("headers: {}", header_map.as_ref().unwrap()["token"]);
        }
    }

    return header_map;
}

fn get_bmnet_string(nc_str: *const c_char) -> String {
    // c传入的指针需要判空
    if nc_str.is_null() {
        return String::from("");
    }

    let c_str = unsafe { CStr::from_ptr(nc_str) };

    let r_str = match c_str.to_str() {
        Ok(text) => text,
        Err(e) => "",
    };

    String::from(r_str)
}

// http/https get请求
#[no_mangle]
pub extern "C" fn bmnet_get(url: *const c_char, headers: *const c_char, cb: Option<Callback>, ext: usize) {
    // c传入的指针需要判空
    // if url.is_null() {
    //     return;
    // }

    // let c_url = unsafe { CStr::from_ptr(url) };

    // let str_url = match c_url.to_str() {
    //     Ok(text) => text,
    //     Err(e) => "",
    // };

    // // url为空，返回
    // if str_url.is_empty() {
    //     return;
    // }

    let str_url = get_bmnet_string(url);

    // 获取header map
    let header_map = get_header(headers);
    
    let result = get_url(str_url, header_map);
    match result {
        Ok(text) => {
            println!("http get: {:?}", text);
            exec_string_callback(cb, text, ext);
        },
        Err(e) => println!("http error: {:?}", e),
    }

    println!("main get");
}

// http/https post请求
#[no_mangle]
pub extern "C" fn bmnet_post(url: *const c_char, 
    headers: *const c_char, 
    data: *const c_char, 
    cb: Option<Callback>, ext: usize) {

    let str_url = get_bmnet_string(url);

    // 获取header map
    let header_map = get_header(headers);

    let str_data = get_bmnet_string(data);
    
    let result = post_url(str_url, header_map, str_data);
    match result {
        Ok(text) => {
            println!("http post: {:?}", text);
            exec_string_callback(cb, text, ext);
        },
        Err(e) => println!("http error: {:?}", e),
    }

    println!("main post");
}

#[no_mangle]
pub extern "C" fn bmnet_timeout(time: u64) {    
    Timer::timeout(time);
}