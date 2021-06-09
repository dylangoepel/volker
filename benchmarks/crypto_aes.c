 #include <string.h>

#include "crypto/aes.h"
#include "benchmark.h"
#include <openssl/rand.h>

int main(){
    benchmark b;
    crypto_aes_t *aes_enc, *aes_dec;
    unsigned char key[32];
    unsigned char iv[32];
    char *msg = "THIS IS A TEST MESSAGE";
    char *w_buff;

    RAND_bytes(key, CRYPTO_AES_KEY_SIZE_256);
    RAND_bytes(iv, CRYPTO_AES_KEY_SIZE_256);

    aes_enc = crypto_aes_new_cipher(CRYPTO_AES_MODE_ENCRYPTION);
    int err = crypto_aes_set_key_and_iv(aes_enc, key, iv, CRYPTO_AES_MODE_256);

    bm_start(&b);
    w_buff = crypto_aes_encrypt(aes_enc, msg, strlen(msg) +1);
    bm_end(&b);
    bm_write(&b, "encryption with aes 256");

    aes_dec = crypto_aes_new_cipher(CRYPTO_AES_MODE_DECRYPTION);
    int err_r = crypto_aes_set_key_and_iv(aes_dec, key, iv, CRYPTO_AES_MODE_256);

    bm_start(&b);
    char *n_msg = crypto_aes_decrypt(aes_dec, w_buff, strlen(w_buff) +1);
    bm_end(&b);
    bm_write(&b, "decryption with aes 256");

    crypto_aes_cipher_free_(aes_enc);
    crypto_aes_cipher_free_(aes_dec);
    printf("%s\n", n_msg);
}
