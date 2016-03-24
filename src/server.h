/*
 * server.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#define CHECKSUM_INDICATOR 0x01
#define END_OF_LIST 0x02

#include <stdio.h>
#include <string.h>
#include "lib_socket.h"

typedef struct server{
	socket_t* skt;
	FILE* remote_file;
}server_t;

int server_execution(int argc, char* argv[]);
int receive_remote_filename(socket_t* skt, FILE* remote_file);

#endif /* SRC_SERVER_H_ */
