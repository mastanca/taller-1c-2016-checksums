/*
 * server.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "server.h"

static void play_with_socket(server_t server, socket_t* client_skt){
	char buffer[100];
	while (strcmp(buffer, "@exit\n") != 0){
		memset(&buffer, 0, sizeof(buffer));
		socket_receive(client_skt, buffer, sizeof(buffer));
		if (strcmp(buffer, "@exit\n") != 0){
			socket_send(client_skt, buffer, sizeof(buffer));
			printf("Sent %i bytes\n", (int)strlen(buffer));
		}
	}
}


int server_execution(int argc, char* argv[]){
	if (argc != 3){
		return 1;
	}
	char* port = argv[2];
	socket_t acep;
	server_t server;
	server.skt = &acep;
	socket_init(server.skt, NULL, port);
	printf("%s \n", "Socket created!");
	socket_bind(server.skt);
	printf("%s \n", "Socket binded!");
	socket_listen(server.skt, 5);
	printf("%s \n", "Socket listening!");
	socket_t client_skt;
	socket_accept(server.skt, &client_skt);
	printf("%s \n", "Client accepted!");

	receive_remote_filename(&client_skt, server.remote_file);

//	play_with_socket(server, &client_skt);

	socket_destroy(&client_skt);
	printf("%s \n", "Client destroyed!");
	socket_destroy(server.skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

int receive_remote_filename(socket_t* skt, FILE* remote_file){
	// BUG: Only works for filename sizes of 1 digit
	size_t filename_length;
	size_t block_size;

	socket_receive(skt, (char*)&filename_length, sizeof(size_t));

	char *name = malloc(filename_length + 1);
	socket_receive(skt, name, filename_length);
	name[filename_length] = 0;

	/*size_t filename_size = atoi(filename_size_char);
	printf("%u \n", (unsigned int)filename_size);


	char filename[filename_size];
	socket_receive(skt, filename, sizeof(filename));
	printf("%s \n", filename);*/

	socket_receive(skt, (char*)&block_size, sizeof(size_t));

	printf("%u\n%u\n%s\n", (unsigned int)filename_length, (unsigned int)block_size, name);

	free(name);

	return 0;
}
