#include "../h/hash.h"

#include <openssl/sha.h>

void hash_password(const char *password, unsigned char *output_hash) {
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, password, strlen(password));
    SHA256_Final(output_hash, &sha256_ctx);
}
