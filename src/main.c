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

int main(int argc, char *argv[]) {
	char* execution_type = argv[1];
	if (strcmp(execution_type, CLIENT) == 0) {
		client_t client;
		client_init(&client, argv[2], argv[3], argv[4], argv[5], argv[6],
				argv[7]);
		client_run(&client);
		// client_destroy is called inside run due to valgrind errors if done
		// otherwise
	} else if (strcmp(execution_type, SERVER) == 0) {
		if (argc != 3) {
			return EXIT_FAILURE;
		}
		server_t server;
		server_init(&server, argv[2]);
		server_run(&server);
		// server_destroy is called inside run due to valgrind errors if done
		// otherwise
	} else {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
