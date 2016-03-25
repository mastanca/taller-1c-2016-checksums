/*
 * server.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#define CHECKSUM_INDICATOR_STRING "0x01"
#define CHECKSUM_INDICATOR 0x01
#define END_OF_LIST 0x02
#define WINDOW_BYTE_DISPLACEMENT -1


#include <stdio.h>
#include <string.h>
#include "lib_socket.h"
#include "list.h"
#include "lib_checksum.h"
#include "file_handler.h"

typedef struct server{
	socket_t* skt;
	size_t block_size;
	FILE* remote_file;
	list_t checksum_list;
}server_t;

int server_execution(int argc, char* argv[]);
int receive_remote_filename(socket_t* skt, server_t* server);
int receive_checksum_list(socket_t* skt, size_t block_size, server_t* server);
int start_comparison_sequence(server_t* server, socket_t* skt);

#endif /* SRC_SERVER_H_ */
