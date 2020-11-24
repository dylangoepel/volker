#ifndef CLIENT_T
#define CLIENT_T

#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	struct sockaddr_in local_client;
	int socket;
	/*how should we tell which ip/addr to use */
	struct in_addr * addr;
}client_conn;


int client_connect(client_conn *client);
int client_init(client_conn *client, int addr, int port);
#endif
