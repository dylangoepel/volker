#include <openssl/rsa.h>
#include <string.h>
#include <stdbool.h>

#include "crypto/rsa.h"

#define PADDING RSA_PKCS1_OAEP_PADDING
#define KEYLENGTH 4096 /*needs to be changed if you use another key length*/

bool __check_length(int len){
    if(len > KEYLENGTH / 8)
        return true;

    else {
        return false;
    }
}


char *encrypt_rsa(RSA *public, char *msg, unsigned int len){
    char *enc_to = NULL;

    if(__check_length(len))
        return NULL;

    enc_to = malloc(RSA_size(public));
    if(enc_to == NULL)
        return NULL;

    if(RSA_public_encrypt(len, (unsigned char*)msg, (unsigned char *)enc_to, public, PADDING) < 0)
        return NULL;

    return enc_to;

}

char *decrypt_rsa(RSA *private, char *enc_msg){
    char *dec_to = NULL;

    dec_to = malloc(RSA_size(private));
    if(dec_to == NULL)
        return NULL;

    if(RSA_private_decrypt(RSA_size(private), (unsigned char *)enc_msg, (unsigned char *)dec_to, private, PADDING) < 0)
       return NULL;

    return dec_to;

}
