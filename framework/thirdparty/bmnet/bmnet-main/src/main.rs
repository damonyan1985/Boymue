mod web;

// 测试程序
use web::client::{get_url};
use std::ffi::{CString, CStr, c_int};
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
        Ok(text) => println!("http get: {:?}", text),
        Err(e) => println!("http error: {:?}", e),
    }

    let t = Box::new(A{});

    let bt = Box::into_raw(t);

    println!("main end");
}

