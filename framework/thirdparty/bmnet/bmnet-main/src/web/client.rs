use std::collections::HashMap;
use serde_json::{Map, Value};
// HeaderName实现了FromStr，必须导入
use std::str::FromStr;
use reqwest::header::{HeaderMap, HeaderName, HeaderValue};
use std::time::Duration;

fn get_header_map(headerMap: Option<Map<String, Value>>) -> HeaderMap {
    let mut headers = HeaderMap::new();
    match headerMap {
        Some(map) => {
            for (k, v) in map {
                println!("header: key = {}, value={}", k.as_str(), v.as_str().unwrap());
                headers.insert(
                    HeaderName::from_str(k.as_str()).unwrap(), 
                    HeaderValue::from_str(v.as_str().unwrap()).unwrap());
            }
        },
        None => println!("header is null")
    }

    return headers;
}

// http/https get请求
#[tokio::main]
pub async fn get_url(url: String, headerMap: Option<Map<String, Value>>) -> Result<String, Box<dyn std::error::Error>> {
    // let mut headers = HeaderMap::new();
    // match headerMap {
    //     Some(map) => {
    //         for (k, v) in map {
    //             println!("header: key = {}, value={}", k.as_str(), v.as_str().unwrap());
    //             headers.insert(
    //                 HeaderName::from_str(k.as_str()).unwrap(), 
    //                 HeaderValue::from_str(v.as_str().unwrap()).unwrap());
    //         }
    //     },
    //     None => println!("header is null")
    // }

    let headers = get_header_map(headerMap);

    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()
        .unwrap()
        .get(&url)
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}

// http/https post请求
#[tokio::main]
pub async fn post_url(url: String, headerMap: Option<Map<String, Value>>, body: String) -> Result<String, Box<dyn std::error::Error>> {
    let headers = get_header_map(headerMap);

    // 加了问号符，如果出错会自动return
    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()
        .unwrap()
        .post(&url)
        .body(body)
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}