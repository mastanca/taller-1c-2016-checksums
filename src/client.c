/*
 * client.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "client.h"

void play_with_socket(client_t client){
	char input[100];
	char buffer[100];
	while (strcmp(input, "@exit\n") != 0){
		memset(&input, 0, sizeof(input));
		fgets(input, sizeof(input), stdin);
		socket_send(client.skt, input, sizeof(input));
		socket_receive(client.skt, buffer, sizeof(buffer));
		printf("%s", buffer);
	}
}

int client_execution(int argc, char* argv[]){
	client_t client;
	char* hostname = argv[2];
	char* port = argv[3];
	char* old_file_name = argv[4];
	char* new_file_name = argv[5];
	char* remote_file_name = argv[6];
	size_t block_size = atoi(argv[7]);

	socket_t skt;
	client.skt = &skt;
	socket_init(client.skt, hostname, port);
	printf("%s \n", "Socket created!");
	socket_connect(client.skt);
	printf("%s \n", "Socket connected!");

	send_remote_filename(client.skt, remote_file_name, block_size);
//	play_with_socket(client);

	socket_destroy(client.skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

static int send_remote_filename(socket_t* skt, char* filename, size_t block_size){
	size_t filename_size;
	filename_size = strlen(filename);
	int message_size;

	message_size = sizeof(int)*2 + filename_size;
	char message_to_send[message_size];

	sprintf(message_to_send, "%u%s%u", (unsigned int)filename_size, filename, (unsigned int)block_size);

	socket_send(skt, message_to_send, sizeof(message_to_send));

	return 0;
}

int open_file(FILE* file, char* file_route, char* mode){
	file = fopen(file_route, mode);
	if ( file == NULL ) return 1;
	while (!feof(file)){
		int c = fgetc(file);
		if ( c != EOF )
			printf("%c", (char) c);
	 }
	return 0;
}


