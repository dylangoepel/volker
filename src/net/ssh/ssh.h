#ifndef SSH_H
#define SSH_H

#include <libssh/libssh.h>
#include <stdbool.h> 

/* dont know if we should put the informations usr, pwd, hst in a struct or not, is takes the same time. 
   i think, direct in the function is prettier
*/



int _ssh_init(char *hstn, char *usr, ssh_session session);
bool ssh_try_pwd(char *pwd, ssh_session session);

#endif // SSH_H
