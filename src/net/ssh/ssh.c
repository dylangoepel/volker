#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>


/*

TODO : scp func

 */
int
_ssh_init(char *hstn, char *usr, char *pwd, ssh_session session){

  session = ssh_new();
  if(session == NULL){
    return -1;
  }

  /* can and should be changed, now only for debugging*/
  int verbo = SSH_LOG_PROTOCOL;
  int c;
  ssh_options_set(session, SSH_OPTIONS_HOST, hstn);
  ssh_options_set(session, SSH_OPTIONS_USER, usr);
  ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbo);


  if((c = ssh_connect(session)) != SSH_OK){
    fprintf(stderr, "Error: %s\n", ssh_get_error(session));
    ssh_free(session);
    return -2;
  }

  if((c = ssh_userauth_password(session, NULL, pwd)) != SSH_AUTH_SUCCESS){
    fprintf(stderr, "Error: %s\n", ssh_get_error(session));
    ssh_disconnect(session);
    ssh_free(session);
    return -3;
  }

    /*checks if auth works*/
    if(c == SSH_AUTH_SUCCESS){
      return 1;
    }

  return -4;
}
