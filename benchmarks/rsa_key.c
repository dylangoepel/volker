#include <stdio.h>
#include <openssl/rsa.h>

#include "rsa_keygen.h"
#include "benchmark.h"

int main(){
    benchmark b;
    RSA *rsa = NULL;
    char *priv_buff = NULL;
    char *priv_buff_wo = NULL;
    char *pub_buff = NULL;

    bm_start(&b);
    rsa = genearte_rsa_keys(4096);
    bm_end(&b);
    bm_write(&b, "generate 4096 key");

    bm_start(&b);
    priv_buff = export_private_key(rsa, "fu7zdttgfuszdgfs");
    bm_end(&b);
    bm_write(&b, "safed 4096 private key + encryption (without default key)");

    bm_start(&b);
    priv_buff_wo = export_private_key(rsa, NULL);
    bm_end(&b);
    bm_write(&b, "safed 4096 private key + encryption (with default key)");

    bm_start(&b);
    pub_buff = export_public_key(rsa);
    bm_end(&b);
    bm_write(&b, "exported public key");

    bm_start(&b);
    if(write_private_key(rsa, "uf7usduzfsdgfsfd", "test.pem") < 0)
        printf("\n Fuck");
    bm_end(&b);
    bm_write(&b, "wrote private key in test.pem + encryption");

}
