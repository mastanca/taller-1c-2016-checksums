/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <string.h>
#include "client.h"
//#include "lib_checksum.h"
#include "server.h"

#define CLIENT "client"
#define SERVER "server"


int main(int argc, char *argv[]){
// SERVER TESTING
	char* execution_type = argv[1];
	if (strcmp(execution_type, CLIENT) == 0){
		return client_execution(argc, argv);

	}else if(strcmp(execution_type, SERVER) == 0){
		return server_execution(argc, argv);
	} else {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
