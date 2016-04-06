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
	unsigned int block_size;
	FILE* remote_file;
	list_t checksum_list;
}server_t;

int server_execution(int argc, char* argv[]);

#endif /* SRC_SERVER_H_ */
