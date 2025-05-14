#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgpack.h>
#include <openssl/sha.h>

typedef struct {
    char *content;
    uint64_t timestamp;
    uint32_t id;
} Message;

void hash_message(const char *content, char *outputBuffer) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, content, strlen(content));
    SHA256_Final(hash, &sha256);
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
}

int main() {
    Message message;
    message.content = "Hello, world!";
    message.timestamp = 1627847265; // Example timestamp
    message.id = 1; // Example ID

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);
    
    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    
    msgpack_pack_map(&pk, 3);
    
    msgpack_pack_str(&pk, strlen("content"));
    msgpack_pack_str_body(&pk, "content", strlen("content"));
    
    msgpack_pack_str(&pk, strlen(message.content));
    msgpack_pack_str_body(&pk, message.content, strlen(message.content));
    
    msgpack_pack_str(&pk, strlen("timestamp"));
    msgpack_pack_str_body(&pk, "timestamp", strlen("timestamp"));
    
    msgpack_pack_uint64(&pk, message.timestamp);
    
    msgpack_pack_str(&pk, strlen("id"));
    msgpack_pack_str_body(&pk, "id", strlen("id"));
    
    msgpack_pack_uint32(&pk, message.id);
    
    printf("Serialized MessagePack data:\n");
    
    for(size_t i = 0; i < sbuf.size; i++) {
        printf("%02x ", (unsigned char)sbuf.data[i]);
    }
    
    printf("\n");

    char hash_output[SHA256_DIGEST_LENGTH * 2 + 1];
    
    hash_message(message.content, hash_output);
    
    printf("Hashed content: %s\n", hash_output);

    msgpack_sbuffer_destroy(&sbuf);

    return 0;
}
