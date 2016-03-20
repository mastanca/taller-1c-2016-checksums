/*
 * client.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "client.h"

int client_execution(int argc, char* argv[]){
	char* hostname = argv[2];
	char* port = argv[3];
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
}
