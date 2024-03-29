use std::collections::HashMap;
use serde_json::{Map, Value};
// HeaderName实现了FromStr，必须导入
use std::str::FromStr;
use reqwest::header::{HeaderMap, HeaderName, HeaderValue};
use std::time::Duration;
use std::borrow::Cow;
use tokio_util::io::ReaderStream;
use tokio_stream::StreamExt;

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

    // .await一般返回result，适用于?表达式
    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()?
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
        .build()?
        .post(&url)
        .body(body)
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}

// post方式上传文件
#[tokio::main]
pub async fn upload_post(url: String, file_path: String, headerMap: Option<Map<String, Value>>, body: String) -> Result<String, Box<dyn std::error::Error>> {
    let headers = get_header_map(headerMap);

    let file_byte = std::fs::read(file_path).unwrap();
    let part = reqwest::multipart::Part::bytes(Cow::from(file_byte));
    let form = reqwest::multipart::Form::new().part("file", part);
    // 加了问号符，如果出错会自动return
    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()?
        .post(&url)
        .multipart(form)
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}

// put方式上传文件
#[tokio::main]
pub async fn upload_put(url: String, file_path: String, headerMap: Option<Map<String, Value>>, body: String) -> Result<String, Box<dyn std::error::Error>> {
    let headers = get_header_map(headerMap);

    let file_byte = std::fs::read(file_path).unwrap();
    let body = reqwest::Body::from(file_byte);
    // 加了问号符，如果出错会自动return
    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()?
        .put(&url)
        .body(body)
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}

#[tokio::main]
pub async fn upload_progress_put(url: String, file_path: String, headerMap: Option<Map<String, Value>>, body: String) -> Result<String, Box<dyn std::error::Error>> {
    let headers = get_header_map(headerMap);

    // 打开文件
    let file = tokio::fs::File::open(&file_path).await.unwrap();
    // 获取文件大小
    let total_size = file.metadata().await.unwrap().len();
    // 创建文件读取流
    let mut reader_stream = ReaderStream::new(file);

    let mut uploaded_size: u64 = 0;
    let async_stream = async_stream::stream! {
        while let Some(chunk) = reader_stream.next().await {
            if let Ok(chunk) = &chunk {
                uploaded_size = uploaded_size + (chunk.len() as u64);
                println!("uploaded_size is {}", uploaded_size);
            }
            yield chunk;
        }
    };
    // 加了问号符，如果出错会自动return
    let resp = reqwest::Client::builder()
        .timeout(Duration::from_millis(10000))
        .default_headers(headers)
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()?
        .put(&url)
        .body(reqwest::Body::wrap_stream(async_stream))
        .send()
        .await?
        .text()
        .await?;

    Ok(resp)
}