#include <stdio.h>
#include <openssl/rsa.h>
#include <string.h>

#include "crypto/rsa_genkey.h"
#include "crypto/rsa.h"
#include "benchmark.h"

RSA *generate_rsa_keys(unsigned int key_length);

int main(){
    benchmark b;
    RSA *rsa = NULL;
    char *msg = "This is a test message";
    int len = strlen(msg);
    char *enc_msg, *dec_msg;

    rsa = generate_rsa_keys(4096);

    bm_start(&b);
    enc_msg = encrypt_rsa(rsa, msg, len);
    bm_end(&b);
    bm_write(&b, "encryted msg with RSA");

    bm_start(&b);
    dec_msg = decrypt_rsa(rsa, enc_msg);
    bm_end(&b);
    bm_write(&b, "decrypted msg with RSA");

    if(enc_msg ==NULL)
        printf("%s\n","error");

    printf("%s\n", dec_msg);

}
