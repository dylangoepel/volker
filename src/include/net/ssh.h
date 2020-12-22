#ifndef SSH_H
#define SSH_H

#include <libssh/libssh.h>
#include <stdbool.h> 



ssh_session *conn_ssh(char **hst, char **usr, int *ver);
bool ssh_pw(char **pw, ssh_session *session);

#endif // SSH_H
