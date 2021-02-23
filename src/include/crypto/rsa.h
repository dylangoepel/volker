#ifndef RSA_H_
#define RSA_H_

#include <openssl/rsa.h>

char *encrypt_rsa(RSA *public, char *msg, unsigned int len);
char *decrypt_rsa(RSA *private, char * enc_msg);

#endif // RSA_H_
