#ifndef RSA_GENKEY_H_
#define RSA_GENKEY_H_

#include <openssl/rsa.h>
#include <openssl/pem.h>

int write_private_key(RSA *rsa, char *pw_in, char *fname);
char *export_private_key(RSA *rsa, char *pw_in);
RSA *import_private_key(char *priv_key, char *pw_in);
RSA *import_public_key(char *pub_key);
char *export_public_key(RSA *pub);
RSA *genearte_rsa_keys(unsigned int key_length);


#endif // RSA_GENKEY_H_
