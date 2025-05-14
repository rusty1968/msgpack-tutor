use serde::{Serialize, Deserialize};
use rmp_serde::{encode, decode};
use sha2::{Sha256, Digest};
use tokio::sync::mpsc;
use tokio::task;

#[derive(Serialize, Deserialize, Debug)]
struct Message {
    content: String,
    timestamp: u64,
    id: u32,
}

#[derive(Debug)]
struct HashedMessage {
    original: String,
    hashed: String,
}

async fn hash_server(mut rx: mpsc::Receiver<Vec<u8>>) {
    while let Some(encoded_message) = rx.recv().await {
        // Decode the MessagePack to the Message struct
        let message: Message = decode::from_slice(&encoded_message).unwrap();

        // Hash the message content
        let mut hasher = Sha256::new();
        hasher.update(message.content.as_bytes());
        let result = hasher.finalize();
        let hashed_message = HashedMessage {
            original: message.content,
            hashed: format!("{:x}", result),
        };
        println!("Received: {}, {}, {:?}", message.id , message.timestamp, hashed_message);
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let (tx, rx) = mpsc::channel(32);

    // Spawn the hash server
    task::spawn(hash_server(rx));

    // Create a message
    let message = Message {
        content: "Hello, world!".to_string(),
        timestamp: 1627847265, // Example timestamp
        id: 1, // Example ID
    };

    // Serialize the message to MessagePack
    let mut buf = Vec::new();
    encode::write(&mut buf, &message)?;

    // Send the encoded message to the server
    tx.send(buf).await?;

    Ok(())
}
