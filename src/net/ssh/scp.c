#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "net/ssh/ssh.h"
#include "net/ssh/ssh_local.h"
/* Only use the scp_write func, scp_write_file should be only local and not global and only used by scp_write*/

int scp_write(ssh_session session){

  ssh_scp scp;
  int rc;

  scp = ssh_scp_new(session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");

  if(scp==NULL){
    fprintf(stderr, "not able to allocate scp session: %s\n", ssh_get_error(session));
    return SSH_ERROR;
  }

  rc = ssh_scp_init(scp);

  if(rc!=SSH_OK){
    fprintf(stderr, "not able to init scp session: %s\n", ssh_get_error(session));
    ssh_scp_free(scp);
    return rc;
  }

  if(scp_write_file(session, scp) < 0){
    return rc;
  }
  
  return SSH_OK; // or 1;
}


int scp_write_file(ssh_session session, ssh_scp scp){

  /* TODO : read binaray file and put it in a char *exe (the best way  would be with fgetc but it needs to be in a char not int)  
     and give it to the function */

  int rc;
  char *exe;
  int len = strlen(exe);
  //create dir in ~/.conf_local
  rc = ssh_scp_push_directory(scp, ".conf_local", 0755);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create dir: %s\n", ssh_get_error(session));
    return rc;
  }
  
  rc = ssh_scp_push_file(scp, ".update", len, 0755 | 0755);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create file in remote: %s\n", ssh_get_error(session));
    return rc;
  }

  rc = ssh_scp_write(scp, exe, len);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to write to remote: %s\n", ssh_get_error(session));
    return rc;
  }

  return SSH_OK; // or 1;

}
