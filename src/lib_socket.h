/*
 * lib_socket.h
 *
 *  Created on: Mar 18, 2016
 *      Author: mastanca
 */

#ifndef SRC_LIB_SOCKET_H_
#define SRC_LIB_SOCKET_H_

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
	int fd;
}socket_t;

int socket_init(socket_t* skt);




#endif /* SRC_LIB_SOCKET_H_ */
