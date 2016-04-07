/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <string.h>
#include "client.h"
#include "server.h"

#define CLIENT "client"
#define SERVER "server"

int main(int argc, char *argv[]){
	char* execution_type = argv[1];
	if (strcmp(execution_type, CLIENT) == 0){
		client_t client;
		client_init(&client, argv);
		client_run(&client);
		client_destroy(&client);
	}else if (strcmp(execution_type, SERVER) == 0){
		server_t server;
		server_init(&server, argc, argv);
		server_run(&server);
		server_destroy(&server);
	} else {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
