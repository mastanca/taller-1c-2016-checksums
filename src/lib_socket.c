/*
 * lib_socket.c
 *
 *  Created on: Mar 18, 2016
 *      Author: mastanca
 */

#include "lib_socket.h"

int socket_init(socket_t* skt){
	int s = 0;
	struct addrinfo hints;
	struct addrinfo *ptr;
	const char *serviceName = "http";


	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = 0;     	/* 0 (or AI_PASSIVE for server)           */

	s = getaddrinfo(NULL, serviceName, &hints, &ptr);

	if (s != 0) {
		printf("Error in getaddrinfo: %s\n", gai_strerror(s));
		return 1;
	}

	skt->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	return 0;
}

