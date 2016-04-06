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
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

typedef struct socket{
	int fd;
	struct addrinfo* result;
}socket_t;

// Initiates the socket with the given parameters
int socket_init(socket_t* skt, char* hostname, char* port);
// Frees socket's resources
int socket_destroy(socket_t* skt);
// Binds the socket to a connection
int socket_bind(socket_t* skt);
// Listens for new connections, up to max clients (blocker)
int socket_listen(socket_t* skt, int max_clients);
// Accepts new connections, returns connection fd in client_skt
int socket_accept(socket_t* skt, socket_t* client_skt);
// Connects to server (blocker)
int socket_connect(socket_t* skt);
// Receives size bytes in buffer from the net
int socket_receive(socket_t* skt, char* buffer, int size);
// Sends size bytes from buffer over the net
int socket_send(socket_t* skt, char* buffer, int size);



#endif /* SRC_LIB_SOCKET_H_ */
