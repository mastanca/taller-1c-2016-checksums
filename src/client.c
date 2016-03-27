
/*
 * client.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "client.h"

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

	if (socket_connect(client.skt) == 0){
		// Open new file
		client.new_file = NULL;
		client.new_file = fopen(new_file_name, "w");

		send_remote_filename(client.skt, remote_file_name, client.block_size);

		// Open old file
		client.old_file = NULL;
		client.old_file = fopen(old_file_name, "r");
		if (client.old_file != NULL){
			send_file_chunks(&client, client.old_file, client.block_size);
		}

		receive_server_response(&client);

	//	play_with_socket(client);
		fclose(client.old_file);
		fclose(client.new_file);
	}


	socket_destroy(client.skt);
	return EXIT_SUCCESS;
}

int receive_server_response(client_t* client){
	// Receive server code
	char server_code = -1;
	while (server_code != EOF_INDICATOR){
		socket_receive(client->skt, (char*)&server_code, sizeof(char));

		if (server_code == NEW_BYTES_INDICATOR){
			receive_new_bytes(client);
		} else if (server_code == BLOCK_FOUND_INDICATOR){
			receive_existing_block(client);
		}
	}
	printf("RECV End of file\n");

	return EXIT_SUCCESS;
}

int receive_new_bytes(client_t* client){
	int new_bytes_longitude = 0;
	socket_receive(client->skt, (char*)&new_bytes_longitude,
			sizeof(new_bytes_longitude));
	// Weird bug when using stack, so malloc!
	char* new_bytes_buffer = malloc(new_bytes_longitude);
	memset(new_bytes_buffer, 0, new_bytes_longitude);
	socket_receive(client->skt, new_bytes_buffer, new_bytes_longitude);

	printf("RECV File chunk %i bytes\n", new_bytes_longitude);
	fwrite(new_bytes_buffer, sizeof(char), new_bytes_longitude, client->new_file);
	free(new_bytes_buffer);
	return EXIT_SUCCESS;
}

int receive_existing_block(client_t* client){
	int existing_block_index = -1;
	socket_receive(client->skt, (char*)&existing_block_index,
			sizeof(existing_block_index));

	printf("RECV Block index %i\n", existing_block_index);
	fseek(client->old_file, client->block_size * existing_block_index,
			SEEK_SET);

	char old_bytes_buffer[client->block_size];
	bool read_something = false;
	read_from_file(client->old_file, old_bytes_buffer, client->block_size,
		 &read_something);
	fwrite(&old_bytes_buffer, sizeof(char), sizeof(old_bytes_buffer),
	 client->new_file);
	return EXIT_SUCCESS;
}

int send_remote_filename(socket_t* skt, char* filename,
		unsigned int block_size){
	int filename_length = strlen(filename);
	char *buffer = malloc(filename_length + 2 * sizeof(int));

	memcpy(buffer, &filename_length, sizeof(int));
	memcpy(buffer + sizeof(int), filename, filename_length);
	memcpy(buffer + sizeof(int) + filename_length, &block_size, sizeof(int));

	socket_send(skt, buffer, filename_length + 2 * sizeof(int));

	free(buffer);

	return EXIT_SUCCESS;
}

int send_file_chunks(client_t* client, FILE* file, unsigned int block_size){
	bool read_something = false;
	checksum_t checksum;
	char buffer[block_size];
	while(!feof(file)){
		read_from_file(file, buffer, block_size, &read_something);
		if (strcmp(buffer, "") != 0) {
			char code = CHECKSUM_INDICATOR;
			socket_send(client->skt, (char*)&code, sizeof(code));
			set_checksum(&checksum, buffer, block_size);
			int number_to_send = checksum.checksum;
			socket_send(client->skt, (char*)&number_to_send, sizeof(number_to_send));
			memset(buffer, 0, sizeof(buffer));
		}
	}
	int code = END_OF_LIST;
	socket_send(client->skt, (char*)&code, sizeof(code));
	return EXIT_SUCCESS;
}


