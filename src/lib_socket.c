/*
 * lib_socket.c
 *
 *  Created on: Mar 18, 2016
 *      Author: mastanca
 */

#include "lib_socket.h"

int socket_init (socket_t* skt, char* hostname, char* port){
	int s = 0;
	struct addrinfo hints;

	if (hostname == NULL){
		hostname = "localhost";
	}

	const char *serviceName = port;


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

int socket_destroy (socket_t* skt){
	if (shutdown(skt->fd, SHUT_RDWR) == -1){
		handle_error("destroy (shutdown)");
		return 1;
	}
	if (close(skt->fd) == -1){
		handle_error("destroy (close)");
		return 1;
	}
	return 0;
}

int socket_bind (socket_t* skt){
	if (bind(skt->fd, skt->result->ai_addr, skt->result->ai_addrlen ) == -1){
		handle_error("bind");
		close(skt->fd);
		freeaddrinfo(skt->result);
		return 1;
	}
	freeaddrinfo(skt->result);
	return 0;
}

int socket_listen (socket_t* skt, int max_clients) {
	if (listen(skt->fd, max_clients) == -1){
		handle_error("listen");
		return 1;
	}
	return 0;
}

int socket_accept (socket_t* skt, socket_t* client_skt) {
	client_skt->fd = accept(skt->fd, NULL, NULL);
	if (client_skt->fd == -1){
		handle_error("accept");
		return 1;
	}
	return 0;
}

int socket_connect (socket_t* skt) {
	if (connect(skt->fd, skt->result->ai_addr, skt->result->ai_addrlen) == -1){
		handle_error("connect");
		close(skt->fd);
		return 1;
	}
	return 0;
}

int socket_receive (socket_t* skt, char* buffer, int size) {
	int received = 0;
	int response = 0;
	bool is_a_valid_socket = true;

	while (received < size && is_a_valid_socket) {
		response = recv(skt->fd, &buffer[received], size-received, MSG_NOSIGNAL);

		if (response == 0){
			// Socket was closed
			is_a_valid_socket = false;
		}else if (response < 0) {
			// There was an error
			is_a_valid_socket = false;
		} else {
			received += response;
		}
	}

	if (is_a_valid_socket) {
		return received;
	} else {
		return -1;
	}

	return 0;
}

int socket_send (socket_t* skt, char* buffer, int size) {
	int sent =0;
	int response = 0;
	bool is_a_valid_socket = true;

	while (sent < size && is_a_valid_socket) {
		response = send(skt->fd, &buffer[sent], size-sent, MSG_NOSIGNAL);

		if (response == 0){
			// Socket was closed
			is_a_valid_socket = false;
		}else if (response < 0) {
			// There was an error
			is_a_valid_socket = false;
		} else {
			sent += response;
		}
	}

	if (is_a_valid_socket) {
		return sent;
	} else {
		return -1;
	}

	return 0;
}

int handle_error(char* function_name){
	fprintf(stderr, "Error on %s: ", function_name);
	fprintf(stderr, "%s\n", strerror(errno));
	return 0;
}

