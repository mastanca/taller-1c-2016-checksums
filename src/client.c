/*
 * client.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "client.h"

// Client tells server which file is going to target and the block size
// to use
static int client_point_target_file(socket_t* skt, char* filename,
		unsigned int block_size);
// Send file checksums of block size to server
static int send_file_chunks(client_t* client, FILE* file,
		unsigned int block_size);
// Receive bytes not available in local file
static int receive_new_bytes(client_t* client);
// Receive existing local block number
static int receive_existing_block(client_t* client);
// Parse response from the server
static int receive_server_response(client_t* client);

int client_init(client_t* client, char* arguments[]) {
	client->hostname = arguments[2];
	client->port = arguments[3];

	client->old_file_name = arguments[4];
	client->new_file_name = arguments[5];
	client->remote_file_name = arguments[6];
	client->block_size = atoi(arguments[7]);

	socket_t skt;
	client->skt = &skt;
	socket_init(client->skt, client->hostname, client->port);
	socket_connect(client->skt);
	// Open new file
	client->new_file = NULL;
	client->new_file = fopen(client->new_file_name, "wb");

	// Open old file
	client->old_file = NULL;
	client->old_file = fopen(client->old_file_name, "rb");

	return EXIT_SUCCESS;
}

int client_destroy(client_t* client) {
	fclose(client->old_file);
	fclose(client->new_file);
	socket_destroy(client->skt);
	return EXIT_SUCCESS;
}

int client_run(client_t* client) {
	client_point_target_file(client->skt, client->remote_file_name,
			client->block_size);

	if (client->old_file != NULL) {
		send_file_chunks(client, client->old_file, client->block_size);
	}
	receive_server_response(client);
	return EXIT_SUCCESS;
}

static int receive_server_response(client_t* client) {
	// Receive server code
	char server_code = -1;
	while (server_code != EOF_INDICATOR) {
		socket_receive(client->skt, (char*) &server_code, sizeof(char));

		if (server_code == NEW_BYTES_INDICATOR) {
			receive_new_bytes(client);
		} else if (server_code == BLOCK_FOUND_INDICATOR) {
			receive_existing_block(client);
		}
	}

	printf("RECV End of file\n");

	return EXIT_SUCCESS;
}

static int receive_new_bytes(client_t* client) {
	uint32_t new_bytes_longitude = 0;
	socket_receive(client->skt, (char*) &new_bytes_longitude,
			sizeof(new_bytes_longitude));
	// Weird bug when using stack, so malloc!
	new_bytes_longitude = ntohl(new_bytes_longitude);
	char* new_bytes_buffer = malloc(new_bytes_longitude);
	memset(new_bytes_buffer, 0, new_bytes_longitude);
	socket_receive(client->skt, new_bytes_buffer, new_bytes_longitude);

	printf("RECV File chunk %i bytes\n", new_bytes_longitude);
	fwrite(new_bytes_buffer, sizeof(char), new_bytes_longitude,
			client->new_file);
	free(new_bytes_buffer);
	return EXIT_SUCCESS;
}

static int receive_existing_block(client_t* client) {
	uint32_t existing_block_index = -1;
	socket_receive(client->skt, (char*) &existing_block_index,
			sizeof(existing_block_index));

	printf("RECV Block index %i\n", existing_block_index);
	fseek(client->old_file, client->block_size * existing_block_index,
	SEEK_SET);

	char* old_bytes_buffer = calloc(client->block_size + 1, sizeof(char));
	if (fread(old_bytes_buffer, sizeof(char), client->block_size,
			client->old_file) != 0) {
		free(old_bytes_buffer);
		return EXIT_FAILURE;
	}
	fwrite(old_bytes_buffer, sizeof(char), strlen(old_bytes_buffer),
			client->new_file);
	free(old_bytes_buffer);
	return EXIT_SUCCESS;
}

static int client_point_target_file(socket_t* skt, char* filename,
		unsigned int block_size) {
	uint32_t filename_length = strlen(filename);
	char *buffer = malloc(filename_length + 2 * sizeof(int));

	// Care about endiannes
	uint32_t net_filename_length = htonl(filename_length);
	block_size = htonl(block_size);

	memcpy(buffer, &net_filename_length, sizeof(int));
	memcpy(buffer + sizeof(int), filename, filename_length);
	memcpy(buffer + sizeof(int) + filename_length, &block_size, sizeof(int));

	socket_send(skt, buffer, filename_length + 2 * sizeof(int));

	free(buffer);

	return EXIT_SUCCESS;
}

static int send_file_chunks(client_t* client, FILE* file,
		unsigned int block_size) {
	checksum_t checksum;
	char* buffer = calloc(block_size + 1, sizeof(char));
	while (!feof(file)) {
		if (fread(buffer, sizeof(char), block_size, file) != 0) {
			char code = CHECKSUM_INDICATOR;

			socket_send(client->skt, (char*) &code, sizeof(code));
			checksum_init(&checksum);
			checksum_set(&checksum, buffer, block_size);
			int number_to_send = get_checksum(&checksum);

			socket_send(client->skt, (char*) &number_to_send,
					sizeof(number_to_send));
			memset(buffer, 0, strlen(buffer));
		}
	}
	int code = END_OF_LIST;
	socket_send(client->skt, (char*) &code, sizeof(code));
	free(buffer);
	return EXIT_SUCCESS;
}

