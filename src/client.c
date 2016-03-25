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
	//char* new_file_name = argv[5];
	char* remote_file_name = argv[6];
	size_t block_size = atoi(argv[7]);

	socket_t skt;
	client.skt = &skt;
	socket_init(client.skt, hostname, port);
	printf("%s \n", "Socket created!");
	socket_connect(client.skt);
	printf("%s \n", "Socket connected!");

	send_remote_filename(client.skt, remote_file_name, block_size);

	// Open old file
	FILE* old_file = NULL;
	old_file = fopen(old_file_name, "r");
	if ( old_file != NULL){
		send_file_chunks(&client, old_file, block_size);
	}

//	play_with_socket(client);
	fclose(old_file);
	socket_destroy(client.skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

int send_remote_filename(socket_t* skt, char* filename, size_t block_size){
	size_t filename_length = strlen(filename);
	char *buffer = malloc(filename_length + 2 * sizeof(size_t));

	memcpy(buffer, &filename_length, sizeof(size_t));
	memcpy(buffer + sizeof(size_t), filename, filename_length);
	memcpy(buffer + sizeof(size_t) + filename_length, &block_size, sizeof(size_t));

	socket_send(skt, buffer, filename_length + 2 * sizeof(size_t));

	free(buffer);

	return 0;
}

int prepare_checksum_for_sending(char* buffer, checksum_t* checksum){
	memset(buffer, 0, strlen(buffer));
	sprintf(buffer, "%d", CHECKSUM_INDICATOR);
	char tmp_buffer[sizeof(checksum->checksum)];
	sprintf(tmp_buffer, "%04lx", checksum->checksum);
	strcat(buffer, tmp_buffer);
	return 0;
}

int send_file_chunks(client_t* client, FILE* file, size_t block_size){
	checksum_t checksum;
	char buffer[block_size];
	while(!feof(file)){
		read_from_file(file, buffer, block_size);
		if (strcmp(buffer, "") != 0) {
			int code = htonl(CHECKSUM_INDICATOR);
			socket_send(client->skt, (char*)&code, sizeof(CHECKSUM_INDICATOR));
			printf("Sending code %d\n", CHECKSUM_INDICATOR);
			set_checksum(&checksum, buffer, block_size);
			int number_to_send = htonl(checksum.checksum);
			printf("Sending %d, this is %lx bytes \n", (int)checksum.checksum, sizeof((int)checksum.checksum));
			socket_send(client->skt, (char*)&number_to_send, sizeof(number_to_send));
			memset(buffer, 0, sizeof(buffer));
		}
	}
	printf("Sending code %d\n", END_OF_LIST);
	int code = htonl(END_OF_LIST);
	socket_send(client->skt, (char*)&code, sizeof(code));
	return 0;
}


