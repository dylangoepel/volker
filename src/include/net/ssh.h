#ifndef SSH_H
#define SSH_H

#include <libssh/libssh.h>
#include <stdbool.h> 

/* dont know if we should put the informations usr, pwd, hst in a struct or not, is takes the same time. 
   i think, direct in the function is prettier
*/



ssh_session conn_ssh(const char *hst, const char *usr, int ver);
bool ssh_try_pwd(char *pwd, ssh_session session);

#endif // SSH_H
