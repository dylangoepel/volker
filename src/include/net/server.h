#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
    struct sockaddr_in remote;
    int socket;
}srv_connection_ctx;

typedef struct {
    struct sockaddr_in local;
    int listener;
}srv_ctx;

int srv_init(srv_ctx *server, uint16_t srv_port);
int srv_listen(srv_ctx *server, void*(*handler)(void *));

#endif

