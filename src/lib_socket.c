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
	const char *hostname = "localhost";
	const char *serviceName = "12345";


	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = 0;     	/* 0 (or AI_PASSIVE for server)           */

	s = getaddrinfo(hostname, serviceName, &hints, &skt->result);

	if (s != 0) {
		fprintf(stderr, "Error in getaddrinfo: %s\n", gai_strerror(s));
		return 1;
	}

	skt->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (skt->fd == -1){
		handle_error("init");
		return 1;
	}
	return 0;
}

int socket_destroy(socket_t* skt){
/*	if (shutdown(skt->fd, SHUT_RDWR) == -1){
		handle_error("destroy (shutdown)");
		return 1;
	}*/
	if (close(skt->fd) == -1){
		handle_error("destroy (close)");
		return 1;
	}
	return 0;
}

int socket_bind(socket_t* skt){
	if (bind(skt->fd, skt->result->ai_addr, skt->result->ai_addrlen ) == -1){
		handle_error("bind");
		close(skt->fd);
		freeaddrinfo(skt->result);
		return 1;
	}
	freeaddrinfo(skt->result);
	return 0;
}

int socket_listen(socket_t* skt, int max_clients){
	if (listen(skt->fd, max_clients) == -1){
		handle_error("listen");
		return 1;
	}
	return 0;
}

int socket_accept(socket_t* skt, socket_t* client_skt){
	client_skt->fd = accept(skt->fd, NULL, NULL);
	if (client_skt->fd == -1){
		handle_error("accept");
		return 1;
	}
	return 0;
}

int socket_connect(socket_t* skt){
	if (connect(skt->fd, skt->result->ai_addr, skt->result->ai_addrlen) == -1){
		handle_error("connect");
		close(skt->fd);
		return 1;
	}
	return 0;
}

int handle_error(char* function_name){
	fprintf(stderr, "Error on %s: ", function_name);
	fprintf(stderr, "%s\n", strerror(errno));
	return 0;
}

