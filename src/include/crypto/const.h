#ifndef DH_CONST_H_
#define DH_CONST_H_

#include <gmp.h>

mpz_t *dh_get_p(); 
mpz_t *dh_get_g(mpz_t *p);

#endif
