#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#include "crypto/rsa.h"

void __free_rsa(RSA *rsa, BIGNUM *bn, BIO *bio){
  if(rsa != NULL)
    RSA_free(rsa);
  else if(bn != NULL)
    BN_free(bn);
  else if (bio != NULL)
    BIO_free_all(bio);
}

bool __check_len(size_t len, size_t bits){
  if(len > bits / 8)
    return true;
  else
    return false;
}

int crypto_rsa_key_is_private(const crypto_rsa_t *env) {
  if(!env || !env->key)
    return 0;

  const BIGNUM *q, *p;
  RSA_get0_factors(env->key, &p, &q);
  return p != NULL;
}

int crypto_rsa_check_private_key(const crypto_rsa_t *env){
  int error;
  error = RSA_check_key(env->key);
  if(error <= 0)
    return 0;
  return 1;
}

//returns true if env contains VLKER_RSA_EXPONENT
int crypto_rsa_public_exponent_ok(const crypto_rsa_t *env){
  const BIGNUM *be, *bn, *bd;
  RSA_get0_key(env->key, &bn, &be, &bd);
  return BN_is_word(be, VLKER_RSA_EXPONENT);
}

crypto_rsa_t *crypto_rsa_new_from_rsa_(RSA *rsa_imp){
  crypto_rsa_t *tmp;
  tmp = malloc(sizeof(crypto_rsa_t *));
  if(tmp == NULL)
    return NULL;

  tmp->cnt_rfs = 1;
  tmp->key = rsa_imp;
  return tmp;
}

crypto_rsa_t *crypto_rsa_inc_rfs_key(crypto_rsa_t *env){
  env->cnt_rfs++;
  return env;
}

char *crypto_rsa_pem_public_encode(crypto_rsa_t *env){
  BIO *pub = BIO_new(BIO_s_mem());
  size_t publen;
  char *tmp_pub;

  if(PEM_write_bio_RSAPublicKey(pub, env->key) < 0)
    return "ERROR 2";

  publen = BIO_pending(pub);
  tmp_pub = malloc(publen + 1);
  if(tmp_pub == NULL)
    return "ERROR 3";

  BIO_read(pub, tmp_pub, publen);
  __free_rsa(NULL, NULL, pub);
  return tmp_pub;
}

char *crypto_rsa_pem_private_encode(crypto_rsa_t *env, char *encpwd){
  if(!crypto_rsa_key_is_private(env))
    return NULL;

  BIO *priv = BIO_new(BIO_s_mem());
  size_t privlen, pwdlen;
  char *tmp_priv;

  if(encpwd != NULL){
    pwdlen = strlen(encpwd);
    if(pwdlen < PASSWORD_MIN_LENGTH)
      return NULL;

    if(PEM_write_bio_RSAPrivateKey(priv, env->key, EVP_aes_256_cbc(), (unsigned char *)encpwd, (int)pwdlen, NULL, NULL) != 1)
      return NULL;

    privlen = BIO_pending(priv);
    tmp_priv = malloc(privlen +1);
    if(tmp_priv ==  NULL)
      return NULL;

    BIO_read(priv, tmp_priv, privlen);
    __free_rsa(NULL, NULL, priv);
    return tmp_priv;
  }
  else{
    pwdlen = strlen(CRYPTO_RSA_PEM_PASSWORD);
    if(PEM_write_bio_RSAPrivateKey(priv, env->key, EVP_aes_256_cbc(), (unsigned char *)CRYPTO_RSA_PEM_PASSWORD, (int)pwdlen, NULL, NULL) != 1)
      return NULL;

    privlen = BIO_pending(priv);
    tmp_priv = malloc(privlen +1);
    if(tmp_priv ==  NULL)
      return NULL;

    BIO_read(priv, tmp_priv, privlen);
    __free_rsa(NULL, NULL, priv);
    return tmp_priv;
  }
}

int crypto_rsa_asn1_public_encode(crypto_rsa_t *pub, char *to, size_t tolen){
  if(!crypto_rsa_public_exponent_ok(pub))
    return -1;

  unsigned char *buf = NULL;
  int len;
  len = i2d_RSAPublicKey(pub->key, &buf);
  if(len < 0 || buf == NULL)
    return -2;

  if((size_t)len > tolen){
    OPENSSL_free(buf);
    return -3;
  }

  memcpy(to, buf, len);
  OPENSSL_free(buf);
  return 1;
}

crypto_rsa_t *crypto_rsa_asn1_public_decode(char *pub, size_t len){
  RSA *rsa;
  unsigned char *buf;
  const unsigned char *cp;
  cp = buf = malloc(len);
  memcpy(buf, pub, len);
  rsa = d2i_RSAPublicKey(NULL, &cp, len);
  free(buf);
  if(!rsa)
    return 0x00;

  return crypto_rsa_new_from_rsa_(rsa);
}

int crypto_rsa_asn1_private_encode(crypto_rsa_t *priv, char *to, size_t tolen){
  if(crypto_rsa_check_private_key(priv) != 1)
    return -1;
  if(!crypto_rsa_key_is_private(priv))
    return -2;

  unsigned char *buf = NULL;
  int len;
  len = i2d_RSAPrivateKey(priv->key, &buf);
  if(len < 0 || buf == NULL)
    return -3;

  if((size_t)len > tolen){
    OPENSSL_free(buf);
    return -4;
  }

  memcpy(to, buf, len);
  OPENSSL_free(buf);
  return 1;
}

crypto_rsa_t *cryptro_rsa_asn1_private_decode(char *priv, size_t len){
  RSA *rsa;
  unsigned char *buf;
  const unsigned char *cp;
  cp = buf = malloc(len);
  rsa = d2i_RSAPrivateKey(NULL, &cp, len);
  free(buf);
  if(!rsa)
    return NULL;

  crypto_rsa_t *res = crypto_rsa_new_from_rsa_(rsa);
  if(crypto_rsa_check_private_key(res) != 1){
    __free_rsa(rsa, NULL, NULL);
    free(res);
    return NULL;
  }
  return res;
}

int crypto_rsa_generate_key_with_bits(crypto_rsa_t *rsa_p, int bits) {
  if(rsa_p->key){
    RSA_free(rsa_p->key);
    rsa_p->key = NULL;
  }
  RSA *tmp = NULL;
  BIGNUM *bne = BN_new();
  if(!bne)
    __free_rsa(NULL, bne, NULL);

  if(!BN_set_word(bne, VLKER_RSA_EXPONENT))
    __free_rsa(NULL, bne, NULL);

  tmp = RSA_new();
  if(!tmp)
    __free_rsa(tmp, bne, NULL);

  if(RSA_generate_key_ex(tmp, bits, bne, NULL) == -1)
    __free_rsa(tmp, bne, NULL);

  rsa_p->key = tmp;
  tmp = NULL;
  if(!rsa_p->key)
    return -1;

  return 1;
}

void crypto_rsa_assign_public(crypto_rsa_t *from, crypto_rsa_t *to){
  RSA_free(to->key);
  to->key = RSAPublicKey_dup(from->key);
}

void crypto_rsa_assign_private(crypto_rsa_t *from, crypto_rsa_t *to){
  RSA_free(to->key);
  to->key = RSAPrivateKey_dup(from->key);
}

//IMPORTANT! you need to malloc to !
int crypto_rsa_public_encrypt(crypto_rsa_t *env, char *to, char *from, size_t fromlen){
  if(__check_len(fromlen, VLKER_RSA_KEY_SIZE))
    return -1;
  if(RSA_public_encrypt((int)fromlen, (unsigned char *)from, (unsigned char *)to, env->key, VLKER_RSA_PADDING) < 0)
    return -2;
  return 1;
}

//IMPORTANT! you need to malloc to !
int crypto_rsa_private_decrypt(crypto_rsa_t *env, char *to, char*from, size_t fromlen){
  if(crypto_rsa_check_private_key(env) != 1)
    return -1;
  if(!crypto_rsa_key_is_private(env))
    return -2;
  if(RSA_private_decrypt((int)fromlen, (unsigned char *)from, (unsigned char *)to, env->key, VLKER_RSA_PADDING) < 0)
    return -3;
  return 1;
}

int crypto_rsa_public_verify(crypto_rsa_t * env, char *to, char *from, size_t fromlen ){
  if(__check_len(fromlen, VLKER_RSA_KEY_SIZE))
    return -1;
  if(RSA_public_decrypt((int)fromlen, (unsigned char *)from, (unsigned char*)to, env->key, VLKER_RSA_PADDING) < 0)
    return -2;
  return 1;
}

int crypto_rsa_private_sign(crypto_rsa_t *env, char *to, char *from, size_t fromlen){
  if(crypto_rsa_check_private_key(env) != 1)
    return -1;
  if(!crypto_rsa_key_is_private(env))
    return -2;
  if(RSA_private_encrypt((int)fromlen, (unsigned char*)from, (unsigned char *)to, env->key, VLKER_RSA_PADDING) < 0)
    return -3;
  return 1;
}
