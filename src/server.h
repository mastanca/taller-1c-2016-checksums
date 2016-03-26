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

typedef struct server{
	socket_t* skt;
	unsigned int block_size;
	FILE* remote_file;
	list_t checksum_list;
}server_t;

int server_execution(int argc, char* argv[]);
int receive_remote_filename(socket_t* skt, server_t* server);
int receive_checksum_list(socket_t* skt, unsigned int block_size,
		server_t* server);
int start_comparison_sequence(server_t* server, socket_t* skt);
int checksum_not_found(char* block, list_t* window_out_bytes, server_t* server,
		checksum_t* checksum);
int send_windowed_bytes(list_t* window_out_bytes, server_t* server,
		socket_t* client_skt);
int send_found_block_number(socket_t* client_skt, unsigned int index);
int send_eof(socket_t* client_skt);

#endif /* SRC_SERVER_H_ */
