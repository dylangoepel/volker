#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <string.h>
#include <errno.h>
//needs to be changed 
#include "server.h"
/*needs to be corrected*/
#include "/src/tpool/tpool.h"

//doesnt matter as long it is not taken
#define INC_MSSG_BUFFER 1000
#define QUEUE_SIZE 10

// TODO: outsource error handling
// TODO : main function shuould create the thread pool
// initialize and prepare a srv_ctx for listening
int srv_init(srv_ctx *server, int port) {
    // initialize thread array
    
    // initialize local sockaddr_in
    server->local.sin_family = AF_INET; // IPv4. TODO: add IPv6 support
    server->local.sin_addr.s_addr = htonl(INADDR_ANY); // no subnet filter (0.0.0.0)
    server->local.sin_port =  htons(port);
    bzero(server->local.sin_zero, sizeof(server->local.sin_zero));

    // create listener socket
    if((server->listener = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("[\e[31mERROR\e[00m] Unable to create listener socket: %s\n", strerror(errno));
        return -1;
    }

    // bind socket
    if(bind(server->listener, (struct sockaddr*)&(server->local), sizeof(struct sockaddr_in)) < 0) {
        printf("[\e[31mERROR\e[00m] Unable to bind listener socket: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

// start listener and run handler on incoming connections
int srv_listen(srv_ctx *server, void(*handler)(void *)) {
    int sock;
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    tpool_t main_pool; //pool name

    tpool_init(&main_pool, 20, 40) /*init the pool and creates 20 threads, this should to the main function*/

    // listen on socket
    if(listen(server->listener, QUEUE_SIZE) < 0) {
        printf("[\e[31mERROR\e[00m] Unable to listen: %s\n", strerror(errno));
        return -1;
    }

    while((sock = accept(server->listener, (struct sockaddr*)&addr, &len) > 0)) {
        pthread_t client_thread;
        srv_connection_ctx *client;

        client = malloc(sizeof(srv_connection_ctx));
        if(client == NULL) {
            printf("[\e[31mERROR\e[00m] Unable to allocate memory: %s\n", strerror(errno));
            continue;
        }

        client->socket = sock;
        /*puts new connection to the list*/

        if (tpool_add_work(main_pool, handler, client) < 0) {
            printf("[\e[31mERROR\e[00m] Unable to add work to thread pool : %s\n", strerror(errno));
            continue;
        }
    }

    return -1;
}
