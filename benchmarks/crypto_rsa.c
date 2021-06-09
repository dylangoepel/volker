#include <stdio.h>
#include <openssl/rsa.h>
#include <string.h>

#include "crypto/rsa.h"
#include "benchmark.h"

int main(){
    benchmark b;
    crypto_rsa_t *rsa;
    rsa = malloc(sizeof(crypto_rsa_t *));
    if(rsa == NULL)
        printf("%s\n", "MALLOC IN  ERROR 1");

    char *msg = "THIS IS A TEST MESSAGE";
    int len = strlen(msg);
    char *enc_msg, *dec_msg;

    if(crypto_rsa_generate_key_with_bits(rsa, VLKER_RSA_KEY_SIZE) < 0)
        printf("%s\n", "ERROR");

    enc_msg = malloc(RSA_size(rsa->key));
    if(enc_msg == NULL)
        printf("%s\n", "ERROR IN MALLOC 2");

    bm_start(&b);
    crypto_rsa_public_encrypt(rsa, enc_msg, msg, len);
    bm_end(&b);
    bm_write(&b, "encryted msg with RSA");

    dec_msg = malloc(RSA_size(rsa->key));
    if(dec_msg == NULL)
        printf("%s\n", "ERROR IN MALLOC 3");

    bm_start(&b);
    crypto_rsa_private_decrypt(rsa, dec_msg, enc_msg, RSA_size(rsa->key));
    bm_end(&b);
    bm_write(&b, "decrypted msg with RSA");

    if(enc_msg ==NULL)
        printf("%s\n","error");

    printf("%s\n", dec_msg);

}
