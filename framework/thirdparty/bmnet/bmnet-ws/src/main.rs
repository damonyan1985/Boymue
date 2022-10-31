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

//#[tokio::main]
async fn say_to_world() -> String {
    println!("say_to_world");
    String::from("say_to_world")
}

fn test_main(receiver: mpsc::Receiver<String>) {
    std::thread::spawn(move || {
        Runtime::new()
            .expect("start tokio runtime error")
            .block_on(async move {
                println!("test_main start"); 
                let url = Url::parse("ws://127.0.0.1:8445/contacts?uid=123").unwrap();
                let (ws_stream, _) = connect_async(url).await.expect("Failed to connect");
                let (write, read) = ws_stream.split();


                let (stdin_tx, stdin_rx) = futures_channel::mpsc::unbounded();
                read_stdin(stdin_tx, receiver);

                let stdin_to_ws = stdin_rx.map(Ok).forward(write);
                let ws_to_stdout = {
                    read.for_each(|message| async {
                        //let data = message.unwrap().into_data();
                        //tokio::io::stdout().write_all(&data).await.unwrap();
                        println!("message: {}", message.unwrap());
                    })
                };

                pin_mut!(stdin_to_ws, ws_to_stdout);
                future::select(stdin_to_ws, ws_to_stdout).await;  

                println!("thread end");
            });
    });
    
}

fn read_stdin(tx: futures_channel::mpsc::UnboundedSender<Message>, receiver: mpsc::Receiver<String>) {
    std::thread::spawn(move || {     
        println!("read_stdin start"); 
        loop {
            match receiver.recv() {
                Ok(message) => {
                    println!("recv: {}", message);
                    tx.unbounded_send(Message::Text(message)).unwrap();
                },
                Err(err) => (),
            }
    
            println!("read_stdin loop");
        }
    });
}



fn main() {
    say_to_world();
    // let (sender, receiver) = mpsc::channel::<String>();
    // test_main(receiver);

    // std::thread::sleep(Duration::from_millis(1000));
    // sender.send("test1".to_string()).expect("send fail");
    // sender.send("test2".to_string()).expect("send fail");
    // sender.send("test3".to_string()).expect("send fail");




    //test_websocket();
    let socket = ws::BMWebSocket::create(String::from("ws://127.0.0.1:8445/contacts?uid=123"));

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
