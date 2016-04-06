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
		return client_execution(argv);
	}else if (strcmp(execution_type, SERVER) == 0){
		server_t server;
		server_init(&server, argc, argv);
		server_run(&server);
		return server_destroy(&server);
	} else {
		return EXIT_FAILURE;
	}
	printf("return2\n");

	return EXIT_SUCCESS;
}
