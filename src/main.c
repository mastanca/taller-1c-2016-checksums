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
	if (strcmp(execution_type, "client") == 0){
		socket_t skt;
		socket_init(&skt);
		printf("%s \n", "Socket created!");
		socket_connect(&skt);
		printf("%s \n", "Socket connected!");
		char input[100];
		char buffer[100];
		while (strcmp(input, "@exit\n") != 0){
			memset(&input, 0, sizeof(input));
			fgets(input, sizeof(input), stdin);
			socket_send(&skt, input, sizeof(input) + 1);
			socket_receive(&skt, buffer, sizeof(buffer));
			printf("%s", buffer);
		}
		socket_destroy(&skt);
		printf("%s \n", "Socket destroyed!");
		return 0;

	}else if(strcmp(execution_type, "server") == 0){
		socket_t acep;
		socket_init(&acep);
		printf("%s \n", "Socket created!");
		socket_bind(&acep);
		printf("%s \n", "Socket binded!");
		socket_listen(&acep, 5);
		printf("%s \n", "Socket listening!");
		socket_t client_skt;
		socket_accept(&acep, &client_skt);
		printf("%s \n", "Client accepted!");
		char buffer[100];
		while (strcmp(buffer, "@exit") != 0){
			memset(&buffer, 0, sizeof(buffer));
			socket_receive(&client_skt, buffer, sizeof(buffer));
			socket_send(&client_skt, buffer, sizeof(buffer));
		}
		socket_destroy(&client_skt);
		printf("%s \n", "Client destroyed!");
		socket_destroy(&acep);
		printf("%s \n", "Socket destroyed!");
		return 0;
	}




}
