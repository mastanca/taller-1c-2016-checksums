/*
 * client.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#define CHECKSUM_INDICATOR 0x01
#define END_OF_LIST 0x02

#include <stdio.h>
#include <string.h>
#include "lib_socket.h"
#include "file_handler.h"
#include "lib_checksum.h"

typedef struct client{
	socket_t* skt;
	FILE* old_file;
}client_t;

int client_execution(int argc, char* argv[]);
int send_remote_filename(socket_t* skt, char* filename, size_t block_size);
int send_file_chunks(client_t* client, FILE* file, size_t block_size);

#endif /* SRC_CLIENT_H_ */
