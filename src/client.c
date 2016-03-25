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
	client.block_size = atoi(argv[7]);

	socket_t skt;
	client.skt = &skt;
	socket_init(client.skt, hostname, port);
	printf("%s \n", "Socket created!");
	socket_connect(client.skt);
	printf("%s \n", "Socket connected!");

	send_remote_filename(client.skt, remote_file_name, client.block_size);

	// Open old file
	client.old_file = NULL;
	client.old_file = fopen(old_file_name, "r");
	if ( client.old_file != NULL){
		send_file_chunks(&client, client.old_file, client.block_size);
	}

	// Open new file
	client.new_file = NULL;
	client.new_file = fopen(new_file_name, "w");

	printf("\n %s \n", "Receving server response...");
	receive_server_response(&client);

//	play_with_socket(client);
	printf("\n %s \n", "Quiting");
	fclose(client.old_file);
	fclose(client.new_file);
	socket_destroy(client.skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

int receive_server_response(client_t* client){
	// Receive server code
	int server_code = 0;
	while (server_code != EOF_INDICATOR){
		socket_receive(client->skt, (char*)&server_code, sizeof(server_code));
		server_code = ntohl(server_code);

		if (server_code == NEW_BYTES_INDICATOR){
			receive_new_bytes(client);
		} else if (server_code == BLOCK_FOUND_INDICATOR){
			receive_existing_block(client);
		}
	}
	printf("RECV End of file");

	return 0;
}

int receive_new_bytes(client_t* client){
	int new_bytes_longitude = 0;
	socket_receive(client->skt, (char*)&new_bytes_longitude, sizeof(new_bytes_longitude));
	new_bytes_longitude = ntohl(new_bytes_longitude);
	char new_bytes_buffer[new_bytes_longitude];
	memset(new_bytes_buffer, 0, sizeof(new_bytes_buffer));
	socket_receive(client->skt, (char*)&new_bytes_buffer, sizeof(new_bytes_buffer));
	printf("RECV File chunk %lu bytes", sizeof(new_bytes_buffer));
//	new_bytes_buffer = ntohl(new_bytes_buffer);
	fwrite(&new_bytes_buffer, sizeof(char), sizeof(new_bytes_buffer), client->new_file);
	return 0;
}

int receive_existing_block(client_t* client){
	int existing_block_index = -1;
	socket_receive(client->skt, (char*)&existing_block_index, sizeof(existing_block_index));
	existing_block_index = ntohl(existing_block_index);
	printf("RECV Block index %i", existing_block_index);
	fseek(client->old_file, client->block_size * existing_block_index , SEEK_SET);
	char old_bytes_buffer[client->block_size];
	read_from_file(client->old_file, old_bytes_buffer, client->block_size);
	fwrite(&old_bytes_buffer, sizeof(char), sizeof(old_bytes_buffer), client->new_file);
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


