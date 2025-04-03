#include "../h/hash.h"

#include <openssl/evp.h>
#include <string.h>

void hash_password(const char *password, unsigned char *output_hash) {
   EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
   const EVP_MD *md = EVP_sha256();
    
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password, strlen(password));
    EVP_DigestFinal_ex(mdctx, output_hash, NULL);
    EVP_MD_CTX_free(mdctx); 
}
