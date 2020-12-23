#ifndef SSH_LOCAL_H
#define SSH_LOCAL_H

#include <libssh/libssh.h>

int  __write_file(ssh_session *session, ssh_scp *scp);

#endif //SSH_LOCAL_H
