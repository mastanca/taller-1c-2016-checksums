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
#include "file_handler.h"
#include "lib_checksum.h"

typedef struct client{
	socket_t* skt;
	FILE* old_file;
	FILE* new_file;
	unsigned int block_size;
}client_t;

int client_execution(int argc, char* argv[]);
int send_remote_filename(socket_t* skt, char* filename,
		unsigned int block_size);
int send_file_chunks(client_t* client, FILE* file, unsigned int block_size);
int receive_new_bytes(client_t* client);
int receive_existing_block(client_t* client);
int receive_server_response(client_t* client);

#endif /* SRC_CLIENT_H_ */
