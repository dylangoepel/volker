#ifndef RSA_H_
#define RSA_H_

#include <openssl/rsa.h>

#define VLKER_RSA_EXPONENT 65537
#define VLKER_RSA_KEY_SIZE 4096
#define VLKER_RSA_PADDING RSA_PKCS1_OAEP_PADDING
#define PASSWORD_MIN_LENGTH 22
#define CRYPTO_RSA_PEM_PASSWORD "duifzthab7udvfldlki7auzvak"

struct crypto_rsa_t {
  int cnt_rfs;
  RSA *key;
};
typedef struct crypto_rsa_t crypto_rsa_t;

int crypto_rsa_key_is_private(const crypto_rsa_t *env);
int crypto_rsa_check_private_key(const crypto_rsa_t *env);
int crypto_rsa_public_exponent_ok(const crypto_rsa_t *env);
crypto_rsa_t *crypto_rsa_new_from_rsa_(RSA *rsa_imp);
crypto_rsa_t *crypto_rsa_inc_rfs_key(crypto_rsa_t *env);
char *crypto_rsa_pem_public_encode(crypto_rsa_t *env);
char *crypto_rsa_pem_private_encode(crypto_rsa_t *env, char *encpwd);
int crypto_rsa_asn1_public_encode(crypto_rsa_t *pub, char *to, size_t tolen);
crypto_rsa_t *crypto_rsa_asn1_public_decode(char *pub, size_t len);
int crypto_rsa_asn1_private_encode(crypto_rsa_t *priv, char *to, size_t tolen);
crypto_rsa_t *cryptro_rsa_asn1_private_decode(char *priv, size_t len);
int crypto_rsa_generate_key_with_bits(crypto_rsa_t *rsa_p, int bits);
void crypto_rsa_assign_public(crypto_rsa_t *from, crypto_rsa_t *to);
void crypto_rsa_assign_private(crypto_rsa_t *from, crypto_rsa_t *to);
int crypto_rsa_public_encrypt(crypto_rsa_t *env, char *to, char *from, size_t fromlen);
int crypto_rsa_private_decrypt(crypto_rsa_t *env, char *to, char*from, size_t fromlen);
int crypto_rsa_public_verify(crypto_rsa_t * env, char *to, char *from, size_t fromlen );
int crypto_rsa_private_sign(crypto_rsa_t *env, char *to, char *from, size_t fromlen);

#endif // RSA_H_
