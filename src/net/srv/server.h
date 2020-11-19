struct sockaddr_in srv, newconn;
void *conneclient_handler(void *);
void server(int port);
int sock_desc, new_socket, *new_sock;