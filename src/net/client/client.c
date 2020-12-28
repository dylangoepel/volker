#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "net/client.h"

/*TODO : give him a addr in client_init via char* or in the struct*/
/*TODO : and create a handler with the tpool */
int __client_connect(client_conn_ctx *client)
{
	if(connect(client->socket,(struct sockaddr*)&(client->local_client),sizeof(struct sockaddr_in)) < 0) {
        perror("Unable to connect to socket:"); 
        return -1;
    }
	return 0;
}


int client_conn(client_conn_ctx *client, char* addr, unsigned int port)
{
  /*intit local_client sockaddr... */ 

    
	client->local_client.sin_family = AF_INET;
	client->local_client.sin_port = htons(port);
	client->local_client.sin_addr.s_addr = inet_addr(addr);

	if((client->socket = socket(AF_INET, SOCK_STREAM, 0)) <0)
	{
		perror("Unable to create socket:"); 
		return -1;
	}

	if(__client_connect(client) < 0)
	{
		perror("Unable to connect to server:");
		return -2;
	}

	/*here with tpool*/
	return 0;

}
