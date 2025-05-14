all: server client

server:
	cargo build --release

client:
	gcc -o client client.c -lmsgpackc -lssl -lcrypto

clean:
	rm -f client
	cargo clean
