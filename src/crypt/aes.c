#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/engine.h>

#include "crypto/aes.h"

crypto_aes_t *crypto_aes_new_from_evp_ctx_(EVP_CIPHER_CTX *env){
    crypto_aes_t *ret;
    ret = malloc(sizeof(crypto_aes_t));
    if(ret == NULL)
        return NULL;

    ret->ctx = env;
    return ret;
}

crypto_aes_t *crypto_aes_new_cipher(bool op_mode) {
    crypto_aes_t *ret;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(ctx == NULL)
        return NULL;

    ret = malloc(sizeof(crypto_aes_t));
    if(ret == NULL)
        return NULL;

    if(EVP_CIPHER_CTX_init(ctx) < 0)
        return NULL;

    if(op_mode)
        ret->aes_mode = CRYPTO_AES_MODE_ENCRYPTION;
    else
        ret->aes_mode = CRYPTO_AES_MODE_DECRYPTION;
    ret->ctx = ctx;
    return ret;
}

int crypto_aes_set_key_and_iv(crypto_aes_t *env, unsigned char *key, unsigned char *iv, int keylen){
    const EVP_CIPHER *cipher_type = 0;
    if(keylen < CRYPTO_AES_MODE_128)
        return -1;
    switch(keylen) {
        case CRYPTO_AES_MODE_128: cipher_type = EVP_aes_128_cbc(); break;
        case CRYPTO_AES_MODE_192: cipher_type = EVP_aes_192_cbc(); break;
        case CRYPTO_AES_MODE_256: cipher_type = EVP_aes_256_cbc(); break;
    }
    if(env->aes_mode == CRYPTO_AES_MODE_ENCRYPTION){
        if(EVP_EncryptInit_ex(env->ctx, cipher_type, NULL,  key, iv) < 0)
            return -2;
    }
    else{
        if(EVP_DecryptInit_ex(env->ctx, cipher_type, NULL, key, iv) < 0)
            return -3;
    }
    if(!env->ctx)
        return -4;

    env->ci_type = cipher_type;
    env->key = key;
    env->iv = iv;
    return 1;
}

char *crypto_aes_encrypt(crypto_aes_t *env, char *from, size_t fromlen){
    unsigned char *ret = NULL;
    int chi_len = (int)fromlen + CRYPTO_AES_BLOCKSIZE, fin_len = 0;

    ret = malloc(chi_len);
    if(ret == NULL)
        return NULL;

    if(EVP_EncryptInit_ex(env->ctx, NULL, NULL, NULL, NULL) < 0)
        return NULL;

    if(EVP_EncryptUpdate(env->ctx, ret, &chi_len, (unsigned char *)from, fromlen) < 0)
        return NULL;

    if(EVP_EncryptFinal_ex(env->ctx, ret + chi_len, &fin_len) < 0)
        return NULL;
    return (char *)ret;
}

char *crypto_aes_decrypt(crypto_aes_t *env, char *from, size_t fromlen){
    unsigned char *ret = NULL;
    int f_len = 0, t_len = (int)fromlen;

    ret = malloc(fromlen);
    if(ret == NULL)
        return NULL;

    if(EVP_DecryptInit_ex(env->ctx, NULL, NULL, NULL, NULL) < 0)
        return NULL;

    if(EVP_DecryptUpdate(env->ctx, ret, &t_len, (unsigned char *)from, fromlen) < 0)
        return NULL;

    if(EVP_DecryptFinal_ex(env->ctx, ret + fromlen, &f_len))
        return NULL;
    return (char *)ret;
}

int crypto_aes_cipher_free_(crypto_aes_t *env) {
    if(!env)
        return -1;
    EVP_CIPHER_CTX_cleanup(env->ctx);
    free(env);
    env = NULL;
    return 1;
}
