/*
 * lib_socket.c
 *
 *  Created on: Mar 18, 2016
 *      Author: mastanca
 */

#include "lib_socket.h"

int socket_init(socket_t* skt, char* hostname, char* port){
	int s = 0;
	struct addrinfo hints;
	int flag = 0;

	if (hostname == NULL || !strcmp(hostname, "127.0.0.1")){
		hostname = NULL;
		flag = AI_PASSIVE;
	}
	// Port is received as a parameter from user, no need to convert to net
	const char *serviceName = port;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = flag;     	/* 0 (or AI_PASSIVE for server)           */

	s = getaddrinfo(hostname, serviceName, &hints, &skt->result);

	if (s != 0) {
		return 1;
	}

	skt->fd = socket(skt->result->ai_family, skt->result->ai_socktype,
			skt->result->ai_protocol);
	if (skt->fd == -1){
		return 1;
	}
	return EXIT_SUCCESS;
}

int socket_destroy(socket_t* skt){
	if (shutdown(skt->fd, SHUT_RDWR) == -1){
		return 1;
	}
	if (close(skt->fd) == -1){
		return 1;
	}
	return EXIT_SUCCESS;
}

int socket_bind(socket_t* skt){
	if (bind(skt->fd, skt->result->ai_addr, skt->result->ai_addrlen) == -1){
		close(skt->fd);
		freeaddrinfo(skt->result);
		return 1;
	}
	freeaddrinfo(skt->result);
	return EXIT_SUCCESS;
}

int socket_listen(socket_t* skt, int max_clients) {
	if (listen(skt->fd, max_clients) == -1){
		return 1;
	}
	return EXIT_SUCCESS;
}

int socket_accept(socket_t* skt, socket_t* client_skt) {
	client_skt->fd = accept(skt->fd, NULL, NULL);
	if (client_skt->fd == -1){
		return 1;
	}
	return EXIT_SUCCESS;
}

int socket_connect(socket_t* skt) {
	int s = 0;
	struct addrinfo *ptr;
	bool are_we_connected = false;
	for (ptr = skt->result; ptr != NULL && are_we_connected == false;
			ptr = ptr->ai_next) {
		s = connect(skt->fd, ptr->ai_addr, ptr->ai_addrlen);
		if (s == -1){
			close(skt->fd);
			skt->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		}
		are_we_connected = (s != -1);
	}
	freeaddrinfo(skt->result);
	if (are_we_connected == false){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int socket_receive(socket_t* skt, char* buffer, int size) {
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
		return -EXIT_FAILURE;
	}
}

int socket_send(socket_t* skt, char* buffer, int size) {
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
		return -EXIT_FAILURE;
	}
}

