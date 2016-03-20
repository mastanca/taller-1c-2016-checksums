/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <stdio.h>
#include <string.h>

#include "lib_socket.h"

int main(int argc, char *argv[])
{
	char* execution_type = argv[1];
	char* port;
	if (strcmp(execution_type, "client") == 0){
		char* hostname = argv[2];
		port = argv[3];
		socket_t skt;
		socket_init(&skt, hostname, port);
		printf("%s \n", "Socket created!");
		socket_connect(&skt);
		printf("%s \n", "Socket connected!");
		char input[100];
		char buffer[100];
		while (strcmp(input, "@exit\n") != 0){
			memset(&input, 0, sizeof(input));
			fgets(input, sizeof(input), stdin);
			socket_send(&skt, input, sizeof(input));
			socket_receive(&skt, buffer, sizeof(buffer));
			printf("%s", buffer);
		}
		socket_destroy(&skt);
		printf("%s \n", "Socket destroyed!");
		return 0;

	}else if(strcmp(execution_type, "server") == 0){
		if (argc != 3){
			return 1;
		}
		port = argv[2];
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




}
