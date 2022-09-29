use std::collections::HashMap;

// http/https get请求
#[tokio::main]
pub async fn get_url(url: String) -> Result<String, Box<dyn std::error::Error>> {
    let resp = reqwest::Client::builder()
        .danger_accept_invalid_certs(true) // 忽略证书验证
        .use_rustls_tls()
        .build()
        .unwrap()
        .get(&url)
        .send()
        .await
        .unwrap()
        .text()
        .await
        .unwrap();

    Ok(resp)
}