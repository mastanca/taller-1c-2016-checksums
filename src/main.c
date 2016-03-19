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
		socket_destroy(&client_skt);
		printf("%s \n", "Client destroyed!");
		socket_destroy(&acep);
		printf("%s \n", "Socket destroyed!");
		return 0;
	}




}
