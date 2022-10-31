use tungstenite::{connect, Message, WebSocket};
use tungstenite::stream::MaybeTlsStream;
use url::Url;
use tokio_tungstenite::{connect_async};
use futures_util::{future, pin_mut, StreamExt};
use std::sync::mpsc;
use tokio::runtime::Runtime;

pub struct BMWebSocket {
    sender: mpsc::Sender<String>,
}

pub trait StringSend {
    fn send_string(&self, msg: String);
}

// 回调给C++
type SocketCallback = unsafe extern "C" fn(data: *const u8, len: usize, ext: usize);

impl BMWebSocket {
    // url: "ws://xxx"
    pub fn create(url: String, cb: Option<SocketCallback>, ext: usize) -> Self {
        let (sender, receiver) = mpsc::channel::<String>();
        Self::ws_start(url, receiver, cb, ext);
        Self {
            sender: sender,
        }
    }

    // 启动websocket
    fn ws_start(url: String, receiver: mpsc::Receiver<String>, cb: Option<SocketCallback>, ext: usize) {
        std::thread::spawn(move || {
            Runtime::new()
                .expect("start tokio runtime error")
                .block_on(async move {
                    println!("socket start"); 
                    let ws_url = Url::parse(&url).unwrap();
                    let (ws_stream, _) = connect_async(ws_url).await.expect("Failed to connect");
                    let (write, read) = ws_stream.split();
    
    
                    let (stdin_tx, stdin_rx) = futures_channel::mpsc::unbounded();
                    Self::ws_recv(stdin_tx, receiver);
    
                    let stdin_to_ws = stdin_rx.map(Ok).forward(write);
                    // 接受socket消息
                    let ws_to_stdout = {
                        read.for_each(|message| async {
                            // 转为[u8]数组切片
                            let bytes = message.unwrap().into_data();
                            println!("message: {}", String::from_utf8(bytes.clone()).expect("Found invalid UTF-8"));
                            match cb {
                                Some(callback) => {
                                    // 获取指针地址， 回调给c++方法接受数据
                                    unsafe { callback(bytes.as_ptr(), bytes.len(), ext); } 
                                },
                                None => {
                                    println!("callback is null")
                                }
                            }
                        })
                    };
    
                    pin_mut!(stdin_to_ws, ws_to_stdout);
                    future::select(stdin_to_ws, ws_to_stdout).await;  
    
                    println!("thread end");
                });
        });
    }

    // 接受发送的socket消息，并发送给服务端
    fn ws_recv(tx: futures_channel::mpsc::UnboundedSender<Message>, receiver: mpsc::Receiver<String>) {
        std::thread::spawn(move || {     
            println!("ws_recv start"); 
            loop {
                match receiver.recv() {
                    Ok(message) => {
                        println!("recv: {}", message);
                        tx.unbounded_send(Message::Text(message)).unwrap();
                    },
                    Err(err) => (),
                }
        
                println!("ws_recv loop");
            }
            println!("ws_recv end");
        });
    }
}

impl StringSend for BMWebSocket {
    // 发送消息
    fn send_string(&self, msg: String) {
        self.sender.send(msg).expect("send fail");
    }
}