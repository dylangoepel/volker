#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>

#define RSA_KEY_EXPO 65537
#define MIN_PW_LENGTH 12

// TODO: fix import_private_key and import_public_key

void __free(RSA *r, BIGNUM *bn, BIO *bio){
    if(r != NULL)
        RSA_free(r);

    else if(bn != NULL)
        BN_free(bn);

    else if(bio != NULL)
        BIO_free_all(bio);
}

int write_private_key(RSA *rsa,  char *pw_in, char *fname){
    FILE *fw;
    char *default_password = ")%e8pU6BL%H6WJ2>"; /*change it before spreading*/
    int def_pw_len = strlen(default_password);

    if(pw_in != NULL){
        int pw_len = strlen(pw_in);

        if(pw_in == 0)
            return -2;

        else if(pw_len < MIN_PW_LENGTH)
            return -3;

        fw = fopen(fname, "w");
        if(PEM_write_RSAPrivateKey(fw, rsa, EVP_aes_256_cbc(), pw_in, pw_len, NULL, NULL) != 1)
            return -4;

        fclose(fw);
        return 1;
    }

    else{
        fw = fopen(fname, "w");
        if(PEM_write_RSAPrivateKey(fw, rsa, EVP_aes_256_cbc(), default_password, def_pw_len, NULL, NULL) != 1)
            return -5;

        fclose(fw);
        return 2;
    }

    return -1;
}

char *export_private_key(RSA *rsa, char *pw_in){
    BIO *wkey = BIO_new(BIO_s_mem());
    char *default_password = ")%e8pU6BL%H6WJ2>"; /*change it before spreading*/
    int def_pw_len = strlen(default_password);
    char *tmpc = NULL;
    size_t wkey_len;

    if(pw_in != NULL){
        int pw_len = strlen(pw_in);

        if(pw_in == 0)
            return NULL;

        else if(pw_len < MIN_PW_LENGTH)
            return NULL;

        if(PEM_write_bio_RSAPrivateKey(wkey, rsa, EVP_aes_256_cbc(), pw_in, pw_len, NULL, NULL) != 1)
            return NULL;

        wkey_len = BIO_pending(wkey);
        tmpc = malloc(wkey_len +1);
        BIO_read(wkey, tmpc, wkey_len);

        __free(NULL, NULL, wkey);
        return tmpc;
    }

    else{
        if(PEM_write_bio_RSAPrivateKey(wkey, rsa, EVP_aes_256_cbc(), default_password, def_pw_len, NULL, NULL) != 1)
            return NULL;

        wkey_len = BIO_pending(wkey);
        tmpc = malloc(wkey_len +1);
        BIO_read(wkey, tmpc, wkey_len);

        __free(NULL, NULL, wkey);
        return tmpc;
    }

    return NULL;
}

RSA *import_private_key(char *priv_key, char *pw_in){

}

RSA *import_public_key(char *pub_key){

}

char *export_public_key(RSA *pub){
    BIO *public = BIO_new(BIO_s_mem());

    if(pub == NULL)
        return NULL;

    if(PEM_write_bio_RSAPublicKey(public, pub) != 1)
        return NULL;

    size_t public_len = BIO_pending(public);
    char *tmp_pub = malloc(public_len +1);

    BIO_read(public, tmp_pub, public_len);

    __free(NULL, NULL, public);
    return tmp_pub;
}


RSA *generate_rsa_keys(unsigned int key_length){
    RSA * tmp = NULL;
    BIGNUM *bn;
    int err;

    bn = BN_new();
    err = BN_set_word(bn, RSA_KEY_EXPO);
    if(err != 1)
        __free(tmp, bn, NULL);

    tmp = RSA_new();
    err = RSA_generate_key_ex(tmp, key_length, bn, NULL);
    if(err != 1)
        __free(tmp, bn, NULL);

    return tmp;
}
