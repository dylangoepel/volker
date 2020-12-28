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

#define MSG_LENGHT 20

/*
Something at recfrom is wrong. Its the client->socket . You can try it by starting this exe and then telneting to localhost 5632

the snd_msg function works with the client but the server cant handle the request and crashes at res == -1

and sometimes it just doesent show anything
*/

int snd_msg(client_conn_ctx * arg){
  char msg[MSG_LENGHT] = "Test";
  if(send(arg->socket, msg, strlen(msg) +1,0) < 0)
    return -1;

  else
    return 1;
}

void rcv_msg(void *arg){
  int res;
  char rec_msg[MSG_LENGHT];
  char lop[MSG_LENGHT] = "Test";
  srv_connection_ctx *client = NULL;
  client = malloc(sizeof(srv_connection_ctx * ));
  
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
    else  if(strcmp(lop, rec_msg))
	printf("%s","Succses");
      
      else
	printf("%s", "Nope");
  }
}


void server(){
  srv_ctx main;
  
  if(srv_init(&main) < 0)
    printf("%s","Error in srv_init");

  if(srv_listen(&main, rcv_msg) < 0)
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

  // you can // tpool init and add and run only server(); as described above
  tpool_t haupt;
  tpool_init(&haupt, 2, 4);
  tpool_add_work(haupt, server, NULL);
  //if you use it like here use sleep
  sleep(1);
  client();
  //server();
  tpool_destroy(haupt, 1);

}
