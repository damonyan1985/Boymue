mod ws;

use tokio;
use tokio::runtime::Runtime;
use std::time::Duration;

use ws::StringSend;
use tungstenite::{connect, Message};
use url::Url;
use tokio_tungstenite::{connect_async};
use futures_util::{future, pin_mut, StreamExt};
use std::sync::mpsc;

fn main() {
    // let (sender, receiver) = mpsc::channel::<String>();
    // test_main(receiver);

    // std::thread::sleep(Duration::from_millis(1000));
    // sender.send("test1".to_string()).expect("send fail");
    // sender.send("test2".to_string()).expect("send fail");
    // sender.send("test3".to_string()).expect("send fail");




    //test_websocket();
    let socket = ws::BMWebSocket::create(String::from("ws://127.0.0.1:8445/contacts?uid=123"), None, 0);

    // //let rt: Runtime = tokio::runtime::Builder::new_current_thread().build().unwrap();
    // let rt: Runtime = tokio::runtime::Builder::new_multi_thread()
    //     .worker_threads(1)
    //     .enable_all()
    //     .build()
    //     .unwrap();
    // rt.spawn(async {
    //     std::thread::sleep(Duration::from_millis(3000));
    //     println!("hello websocket");
    //     say_to_world().await;
    // });

    // rt.spawn(async {
    //     println!("hello websocket1");
    // });

    socket.send_string(String::from("test1"));
    socket.send_string(String::from("test2"));
    socket.send_string(String::from("test3"));

    // println!("hello websocket2");

    //test_main();
    loop {}
}
