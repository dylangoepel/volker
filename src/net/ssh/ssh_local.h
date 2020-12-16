#ifndef SSH_LOCAL_H
#define SSH_LOCAL_H

#include <libssh/libssh.h>

int scp_write_file(ssh_session, ssh_scp scp);

#endif //SSH_LOCAL_H
