/*
 * server.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "server.h"

#define MAX_NUMBER_OF_CLIENTS 1

// Receives the name of the target file, returns 0 on success
static int receive_remote_filename(socket_t* skt, server_t* server);
// Receives a list of checksums, returns 0 on success
static int receive_checksum_list(socket_t* skt, server_t* server);
// Initiates the comparison of checksums from received and target file
static int start_comparison_sequence(server_t* server, socket_t* skt);
// Windows the not found byte and computes the rolling checksum
static int checksum_not_found(server_t* server, char* block, list_t* window_out_bytes,
		checksum_t* checksum);
// Sends the bytes that where windowed to client
static int send_windowed_bytes(list_t* window_out_bytes, socket_t* client_skt);
// Sends the found block number to client
static int send_found_block_number(socket_t* client_skt, unsigned int index);
// Sends EOF code to client
static int send_eof(socket_t* client_skt);
// Returns the size of the checksums list
static int get_size_of_checksum_list();

int server_init(server_t* server, int arguments_count, char* arguments[]){
	if (arguments_count != 3){
		return EXIT_FAILURE;
	}
	socket_t acep;
	server->skt = &acep;
	server->port = arguments[2];

	list_t list;
	// Compiler warning if this values are not zero before list init
	list.capacity = 0;
	list.size = 0;
	list.data = NULL;
	server->checksum_list = list;
	list_init(&(server->checksum_list));
	// Initiate server sockets
	socket_init(server->skt, NULL, server->port);

	socket_bind(server->skt);

	socket_listen(server->skt, MAX_NUMBER_OF_CLIENTS);
	socket_t client_skt;
	server->client_skt = &client_skt;

	socket_accept(server->skt, server->client_skt);
	return EXIT_SUCCESS;
}

int server_destroy(server_t* server){
	socket_destroy(server->client_skt);
	socket_destroy(server->skt);
	fclose(server->remote_file);
	// Free checksum list
	list_free(&(server->checksum_list));
	return EXIT_SUCCESS;
}

int server_run(server_t* server){
	receive_remote_filename(server->client_skt, server);

	receive_checksum_list(server->client_skt, server);

	start_comparison_sequence(server, server->client_skt);

	return EXIT_SUCCESS;
}

static int start_comparison_sequence(server_t* server, socket_t* skt){
	bool read_something = false;
	list_t window_out_bytes;
	list_init(&window_out_bytes);

	// Load new block from file
	char* block = calloc(server->block_size + 1, sizeof(char));
	read_from_file(server->remote_file, block, server->block_size,
			&read_something);

	// Get checksum of the new block
	checksum_t checksum;
	checksum_init(&checksum);
	checksum_set(&checksum, block, strlen(block));


	while(!feof(server->remote_file)){
		int i = 0;
		int found_index = -1;
		while(i < get_size_of_checksum_list(server) && found_index < 0){
			unsigned int i_element = list_get(&server->checksum_list, i);
			if(get_checksum(&checksum) == i_element){
				found_index = i;
			}
			i++;
		}
		if (found_index < 0){
			checksum_not_found(server, block, &window_out_bytes, &checksum);
		}else{
			if (list_get_size(&window_out_bytes) > 0){
				send_windowed_bytes(&window_out_bytes, skt);
			}
			send_found_block_number(skt, found_index);
			read_from_file(server->remote_file, block, strlen(block),
			 &read_something);
			checksum_init(&checksum);
			checksum_set(&checksum, block, strlen(block));
		}
	}
	// If there are remaining windowed bytes send them
	if (list_get_size(&window_out_bytes) > 0 || ((strlen(block) > 0) &&
	 (read_something == true))){
		for (unsigned int i = 0; i < strlen(block); ++i) {
			char remaining_char = block[i];
			list_append(&window_out_bytes, remaining_char);
		}
		send_windowed_bytes(&window_out_bytes, skt);
	}
	free(block);
	list_free(&window_out_bytes);
	send_eof(skt);

	return EXIT_SUCCESS;
}

static int receive_remote_filename(socket_t* skt, server_t* server){
	uint32_t filename_length;
	uint32_t block_size;

	socket_receive(skt, (char*)&filename_length, sizeof(int));
	// Care about endiannes
	filename_length = ntohl(filename_length);

	char *name = malloc(filename_length + 1);
	socket_receive(skt, name, filename_length);

	name[filename_length] = 0;

	socket_receive(skt, (char*)&block_size, sizeof(int));

	// Care about endiannes
	server->block_size = ntohl(block_size);

	// Open remote file here and assign to server_t
	server->remote_file = fopen(name, "r");

	free(name);

	return EXIT_SUCCESS;
}

static int receive_checksum_list(socket_t* skt, server_t* server){
	char code = '\0';
	int checksum = 0;
	while (code != END_OF_LIST){
		socket_receive(skt, (char*)&code, sizeof(code));

		if (CHECKSUM_INDICATOR == code){
			socket_receive(skt, (char*)&checksum, sizeof(checksum));
			list_append(&(server->checksum_list), checksum);
		}
	}
	return EXIT_SUCCESS;
}

static int checksum_not_found(server_t* server, char* block, list_t* window_out_bytes,
		checksum_t* checksum){
	char byte_to_window = block[0];
	list_append(window_out_bytes, byte_to_window);

	// Move cursor block size bytes to the left and return 1
	int index = WINDOW_BYTE_DISPLACEMENT * (server->block_size) +
	 (-1 * WINDOW_BYTE_DISPLACEMENT);
	fseek(server->remote_file, index, SEEK_CUR);
	bool read_something = false;
	read_from_file(server->remote_file, block, server->block_size,
		 &read_something);
	char* rolling_buffer = calloc(server->block_size + 1, sizeof(char));
	rolling_buffer[0] = byte_to_window;
	memcpy(rolling_buffer + strlen(rolling_buffer), block, strlen(block));
	checksum_t old_checksum;
	old_checksum = *checksum;

	checksum_rolling(checksum, &old_checksum, rolling_buffer +1,
			server->block_size);

	free(rolling_buffer);
	return EXIT_SUCCESS;
}

static int send_windowed_bytes(list_t* window_out_bytes, socket_t* skt){
	char* buffer_to_send = calloc(window_out_bytes->size + 1, sizeof(char));
	for (int i = 0; i < window_out_bytes->size; ++i) {
		char i_element = list_get(window_out_bytes, i);
		strncat(buffer_to_send, &i_element, sizeof(char));
	}
	char new_bytes_indicator = NEW_BYTES_INDICATOR;

	socket_send(skt, (char*)&new_bytes_indicator, sizeof(new_bytes_indicator));

	// Send 4 bytes with the length of the new bytes
	int new_bytes_size = htonl(strlen(buffer_to_send));

	socket_send(skt, (char*)&new_bytes_size, sizeof(new_bytes_size));

	// Send the actual bytes
	socket_send(skt, buffer_to_send, strlen(buffer_to_send));
	free(buffer_to_send);
	list_free(window_out_bytes);
	list_init(window_out_bytes);
	return EXIT_SUCCESS;
}

static int send_found_block_number(socket_t* skt, unsigned int index){
	char block_found_indicator = BLOCK_FOUND_INDICATOR;

	socket_send(skt, (char*)&block_found_indicator,
	 sizeof(block_found_indicator));
	int block_number = index;

	socket_send(skt, (char*)&block_number, sizeof(block_number));
	return EXIT_SUCCESS;
}

static int send_eof(socket_t* skt){
	char eof_indicator = EOF_INDICATOR;

	socket_send(skt, (char*)&eof_indicator, sizeof(eof_indicator));
	return EXIT_SUCCESS;
}

static int get_size_of_checksum_list(server_t* server){
	return list_get_size(&(server->checksum_list));
}
