mod web;

use web::client::{get_url};
//use tokio_core::reactor::Core; 

// https://httpbin.org/ip
// fn main() {
//     // Send GET request and inspect result, with proper error handling.
//     if let Err(e) = get(String::from("https://127.0.0.1:8443/user/v1/testlogin")) {
//         eprintln!("FAILED: {}", e);
//         std::process::exit(1);
//     }
// }

fn main() {
    // Send GET request and inspect result, with proper error handling.
    // if let Err(e) = get(String::from("https://127.0.0.1:8443/user/v1/testlogin")) {
    //     eprintln!("FAILED: {}", e);
    //     std::process::exit(1);
    // }

    let result = get_url(String::from("https://127.0.0.1:8443/user/v1/testlogin"));
    match result {
        Ok(text) => println!("http get: {:?}", text),
        Err(e) => println!("http error: {:?}", e),
    }
}

