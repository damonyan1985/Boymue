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

impl BMWebSocket {
    // url: "ws://xxx"
    pub fn create(url: String) -> Self {
        let (sender, receiver) = mpsc::channel::<String>();
        Self::start(url, receiver);
        Self {
            sender: sender,
        }
    }

    // 启动websocket
    fn start(url: String, receiver: mpsc::Receiver<String>) {
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
                            println!("message: {}", message.unwrap());
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