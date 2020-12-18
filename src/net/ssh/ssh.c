#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <errno.h>
#include "net/ssh.h"

bool ssh_try_pwd(char *pwd, ssh_session session){

  int c;

  if(session == NULL){
    return false;
  }

  if((c = ssh_userauth_password(session, NULL, pwd)) != SSH_AUTH_SUCCESS){
		//its maybe not the best way
	    fprintf(stderr, "Error: %s\n", ssh_get_error(session));
	    return false;
  }

  if(c == SSH_AUTH_SUCCESS){
      return true;
  }

  
  return false;

}

ssh_session conn_ssh(const char *hst, const char *usr,int ver){
  ssh_session session;
  session=ssh_new();
  
  if (session == NULL) {
    return NULL;
  }

  if(usr != NULL){
    if (ssh_options_set(session, SSH_OPTIONS_USER, usr) < 0) {
      ssh_disconnect(session);
      return NULL;
    }
  }

  if (ssh_options_set(session, SSH_OPTIONS_HOST, hst) < 0) {
    return NULL;
  }
  ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &ver);
  
  if(ssh_connect(session)){
    fprintf(stderr,"Connection failed : %s\n",ssh_get_error(session));
    ssh_disconnect(session);
    return NULL;
  }
 
  return session;
}
