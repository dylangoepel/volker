#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
#include <string.h>
//needs to be changed 
#include "server.h"
//doesnt matter as long it is not taken
#define PORT 8080
#define INC_MSSG_BUFFER 1000


struct sockaddr_in srv, newconn;
void *conneclient_handler(void *);
void server(int port);
int sock_desc, new_socket, *new_sock;


//The error handling is bs but i dont care 
//This is not final!!!

//main server driver function
void server(int port){

		bzero((char*)&srv, sizeof(srv)); //first byte with null

		/* Assign ip ...*/
		srv.sin_family = AF_INET;
		srv.sin_addr.s_addr = htonl(INADDR_ANY); //is best 
		srv.sin_port =  PORT;
		sock_desc = socket(AF_INET, SOCK_STREAM, 0); //create socket

		if (sock_desc == -1)
		{

			printf("error , could not create socket\n");
			

		}

		if (bind(sock_desc, (struct sockaddr*)&srv, sizeof(srv)) != 0) //bind socket and check if an error exists
			{
				printf("error ...\n");

			}

		listen(sock_desc, 5);

		
		socklen_t newconn_buffer = sizeof(newconn);

		while(new_socket == accept(sock_desc, (struct sockaddr*)&newconn, &newconn_buffer))
		{
			pthread_t clientconn_thread;

				new_sock = malloc(1);
				*new_sock = new_socket;

				/* create a new thread for each new client which is bs!!! but i will fix it later*/
				if (pthread_create(&clientconn_thread, NULL, conneclient_handler, (void *)new_sock) < 0) 
					 {

					 	printf("error");

					 }
		}

					 if (new_socket<0)
						    {
						        perror("accept failed");
						        
						    }
}


void *conneclient_handler(void * sock_desc_session)
{
	/*you can do anything with the data
	we havent chosen what to do yet */

	int socket = *(int *)sock_desc_session;
	int inc_msg;
	char data[INC_MSSG_BUFFER]; //inc mssg buffer


	while(inc_msg == recv(socket, data, INC_MSSG_BUFFER,0)> 0) //recieves new data
	{


		//do anything

	}

	if (inc_msg == 0 )
	{

		printf("error ...");


	}
	else if (inc_msg == -1)
	{

		printf("error ...");


	}

	free(sock_desc_session);
	return 0;

}
