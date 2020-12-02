#ifndef SSH_H
#define SSH_H


/* dont know if we should put the informations usr, pwd, hst in a struct or not, is takes the same time. 
   i think, direct in the function is prettier
*/



int _ssh_init(char *hstn, char *usr, char *pwd, ssh_session session);


#endif // SSH_H
