/*
 * client.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "lib_socket.h"
#include "lib_checksum.h"

typedef struct client {
	socket_t* skt;
	char* hostname;
	char* port;
	char* old_file_name;
	char* new_file_name;
	char* remote_file_name;
	FILE* old_file;
	FILE* new_file;
	unsigned int block_size;
} client_t;

// Initiates client ADT
int client_init(client_t* client, char* hostname, char* port,
		char* old_file_name, char* new_file_name, char* remote_file_name,
		char* block_size);
// Frees client's resources
int client_destroy(client_t* client);
// Run server
int client_run(client_t* client);

#endif /* SRC_CLIENT_H_ */
