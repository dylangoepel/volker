#include <stdio.h>
#include <string.h>
#include <sys/socket.h>                                                                   
#include <netinet/in.h> 
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "net/server.h"
#include "net/client.h"
#include "tpool/tpool.h"
#include "benchmark.h"

#define MSG_LENGHT 20

int snd_msg(client_conn_ctx * arg){
  char msg[MSG_LENGHT] = "Test Nachricht";
  if(send(arg->socket, msg, strlen(msg) +1,0) < 0)
    return -1;

  else
    return 1;
}

void rcv_msg(void *arg){
  int res;
  char rec_msg[MSG_LENGHT];
  char lop[MSG_LENGHT] = "Test Nachricht";
  srv_connection_ctx *client = NULL;
  client = malloc(sizeof(srv_connection_ctx *));
  
  if(client == NULL){
     printf("[\e[31mERROR\e[00m] Unable to allocate memory: %s\n", strerror(errno));
  }
  client = (srv_connection_ctx *)arg;
  for(;;){
    res = recvfrom(client->socket,rec_msg, MSG_LENGHT, 0, NULL, NULL );

    if(res == -1 )
      fprintf(stderr, "No Connection: %s\n", strerror(errno));
    else if(res == 0){
      fprintf(stderr, "Server disconnected: %s\n", strerror(errno));
      break;
    }
    else if(strcmp(lop, rec_msg))
      break;
      
  }
}


void server(){
  srv_ctx main;
  
  if(srv_init(&main) < 0)
    printf("%s","Error in srv_init");

  if(srv_listen(&main, &rcv_msg) < 0)
    printf("%s","Error srv_listen");     
}

void client(){
  client_conn_ctx main;
    
  char *addr = "127.0.0.1";
  unsigned int port = 5632;
  
  if(client_conn(&main, addr, port) < 0)
    printf("%s","Error in client_init");

  if(snd_msg(&main) <0 )
    printf("%s","Error in sending");
  
}


int main(){

  tpool_t haupt;
  benchmark ben;
  tpool_init(&haupt, 2, 4);

  bm_start(&ben);
  tpool_add_work(&haupt, &server, NULL);
  bm_end(&ben);
  bm_write(&ben, "intit and listen server");
  
  sleep(1);
  
  bm_start(&ben);
  client();
  bm_end(&ben);
  bm_write(&ben, "init client and send test msg");

  tpool_destroy(&haupt, 1);

}
