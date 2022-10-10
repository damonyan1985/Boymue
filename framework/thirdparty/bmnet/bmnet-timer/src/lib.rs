use tokio::time;

use std::time::Duration;
use std::thread;
use futures_timer::Delay;
use tokio::runtime::Runtime;

pub struct Timer {}

impl Timer {
    pub fn timeout(time: u64) {
        // move表示，变量实现了copy就会copy，没有实现copy就是move
        thread::spawn(move || {
            Runtime::new()
            .expect("start tokio runtime error")
            .block_on(async move {
                let now = Delay::new(Duration::from_millis(time)).await;
                println!("waited for {} secs", time);
            });
        });
    }
}



