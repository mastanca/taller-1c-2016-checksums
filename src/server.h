/*
 * server.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "lib_socket.h"
#include "list.h"
#include "lib_checksum.h"
#include "file_handler.h"

// Define server ADT
typedef struct server{
	socket_t* skt;
	socket_t* client_skt;
	unsigned int block_size;
	FILE* remote_file;
	list_t checksum_list;
	char* port;
}server_t;

// Initiates server, returns 0 on success
int server_init(server_t* server, int arguments_count, char* arguments[]);
// Starts the server algorithm
int server_run(server_t* server);
// Closes and frees server resources
int server_destroy(server_t* server);

#endif /* SRC_SERVER_H_ */
