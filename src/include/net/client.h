#ifndef CLIENT_T
#define CLIENT_T

#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	struct sockaddr_in local_client;
	int socket;
	/*how should we tell which ip/addr to use */
	struct in_addr * addr;
}client_conn_ctx;


int __client_connect(client_conn_ctx *client);
int client_conn(client_conn_ctx *client, char * addr, unsigned int port);
#endif
