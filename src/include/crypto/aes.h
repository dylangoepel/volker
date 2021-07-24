#ifndef AES_H_
#define AES_H_

#include <openssl/evp.h>
#include <stdbool.h>

#define CRYPTO_AES_KEY_SIZE_256 32
#define CRYPTO_AES_KEY_SIZE_192 24
#define CRYPTO_AES_KEY_SIZE_128 16
#define CRYPTO_AES_BLOCKSIZE 128
#define CRYPTO_AES_MODE_ENCRYPTION true
#define CRYPTO_AES_MODE_DECRYPTION false
#define CRYPTO_AES_MODE_256 256
#define CRYPTO_AES_MODE_192 192
#define CRYPTO_AES_MODE_128 128
#define CRYPTO_AES_IV_SIZE 16

struct crypto_aes_t {
    int ctxcnt;
    EVP_CIPHER_CTX *ctx;
    const EVP_CIPHER *ci_type;
    unsigned char *key;
    unsigned char *iv;
    bool aes_mode; // true -> encryption ; false->decryption
};
typedef struct crypto_aes_t crypto_aes_t;

crypto_aes_t *crypto_aes_new_from_evp_ctx_(EVP_CIPHER_CTX *env);
crypto_aes_t *crypto_aes_new_cipher(bool op_mode);
int crypto_aes_set_key_and_iv(crypto_aes_t *env, unsigned char *key, unsigned char *iv, int keylen);
char *crypto_aes_encrypt(crypto_aes_t *env, char *from, size_t fromlen);
char *crypto_aes_decrypt(crypto_aes_t *env, char *from, size_t fromlen);
int crypto_aes_cipher_free_(crypto_aes_t *env);

#endif // AES_H_
