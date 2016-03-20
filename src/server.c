/*
 * server.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "server.h"

int server_execution(int argc, char* argv[]){

	if (argc != 3){
		return 1;
	}
	char* port = argv[2];
	socket_t acep;
	socket_init(&acep, NULL, port);
	printf("%s \n", "Socket created!");
	socket_bind(&acep);
	printf("%s \n", "Socket binded!");
	socket_listen(&acep, 5);
	printf("%s \n", "Socket listening!");
	socket_t client_skt;
	socket_accept(&acep, &client_skt);
	printf("%s \n", "Client accepted!");
	char buffer[100];
	while (strcmp(buffer, "@exit\n") != 0){
		memset(&buffer, 0, sizeof(buffer));
		socket_receive(&client_skt, buffer, sizeof(buffer));
		if (strcmp(buffer, "@exit\n") != 0){
			socket_send(&client_skt, buffer, sizeof(buffer));
			printf("Sent %i bytes\n", (int)strlen(buffer));
		}
	}
	socket_destroy(&client_skt);
	printf("%s \n", "Client destroyed!");
	socket_destroy(&acep);
	printf("%s \n", "Socket destroyed!");
	return 0;
}
