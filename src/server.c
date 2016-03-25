/*
 * server.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "server.h"

//static void play_with_socket(server_t server, socket_t* client_skt){
//	char buffer[100];
//	while (strcmp(buffer, "@exit\n") != 0){
//		memset(&buffer, 0, sizeof(buffer));
//		socket_receive(client_skt, buffer, sizeof(buffer));
//		if (strcmp(buffer, "@exit\n") != 0){
//			socket_send(client_skt, buffer, sizeof(buffer));
//			printf("Sent %i bytes\n", (int)strlen(buffer));
//		}
//	}
//}

int server_execution(int argc, char* argv[]){
	if (argc != 3){
		return 1;
	}
	char* port = argv[2];
	socket_t acep;
	server_t server;
	server.skt = &acep;

	checksum_list_t checksum_list;
	server.checksum_list = checksum_list;
	checksum_list_init(&server.checksum_list);

	socket_init(server.skt, NULL, port);
	// Avoid time wait
	int option = 1;
	setsockopt(server.skt->fd,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option));
	printf("%s \n", "Socket created!");
	socket_bind(server.skt);
	printf("%s \n", "Socket binded!");
	socket_listen(server.skt, 5);
	printf("%s \n", "Socket listening!");
	socket_t client_skt;
	socket_accept(server.skt, &client_skt);
	printf("%s \n", "Client accepted!");

	receive_remote_filename(&client_skt, &server);

	receive_checksum_list(&client_skt, server.block_size, &server);

//	play_with_socket(server, &client_skt);

	socket_destroy(&client_skt);
	printf("%s \n", "Client destroyed!");
	socket_destroy(server.skt);
	printf("%s \n", "Socket destroyed!");
	// Free checksum list
	checksum_list_free(&server.checksum_list);
	return 0;
}

int receive_remote_filename(socket_t* skt, server_t* server){
	size_t filename_length;
	size_t block_size;

	socket_receive(skt, (char*)&filename_length, sizeof(size_t));

	char *name = malloc(filename_length + 1);
	socket_receive(skt, name, filename_length);
	name[filename_length] = 0;

	socket_receive(skt, (char*)&block_size, sizeof(size_t));

	printf("%u\n%u\n%s\n", (unsigned int)filename_length, (unsigned int)block_size, name);
	server->block_size = block_size;

	// Open remote file here and assign to server_t

	free(name);

	return 0;
}

int receive_checksum_list(socket_t* skt, size_t block_size, server_t* server){
	printf("%s \n", "Receiving checksum list");
	char code[sizeof(char)];
	char tmp_buffer[sizeof(int)];
	while (strcmp(code, "2") != 0){
		socket_receive(skt, code, sizeof(char));
		if (CHECKSUM_INDICATOR == strtol(code, NULL, 10)){
			socket_receive(skt, tmp_buffer, sizeof(tmp_buffer));
			checksum_list_append(&(server->checksum_list), strtol(tmp_buffer, (char**)NULL, 16));
		}
	}
	return 0;
}
