/*
 * server.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "server.h"

int server_execution(int argc, char* argv[]){
	if (argc != 3){
		return 1;
	}
	char* port = argv[2];
	socket_t acep;
	server_t server;
	server.skt = &acep;

	list_t list;
	server.checksum_list= list;
	list_init(&server.checksum_list);

	socket_init(server.skt, NULL, port);
	// Avoid time wait
	int option = 1;
	setsockopt(server.skt->fd,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),
			(char*)&option,sizeof(option));
	socket_bind(server.skt);
	socket_listen(server.skt, 5);
	socket_t client_skt;
	socket_accept(server.skt, &client_skt);

	receive_remote_filename(&client_skt, &server);

	receive_checksum_list(&client_skt, server.block_size, &server);

	start_comparison_sequence(&server, &client_skt);

	socket_destroy(&client_skt);
	socket_destroy(server.skt);
	fclose(server.remote_file);
	// Free checksum list
	list_free(&server.checksum_list);
	return EXIT_SUCCESS;
}

int start_comparison_sequence(server_t* server, socket_t* skt){
	list_t window_out_bytes;
	list_init(&window_out_bytes);

	// Load new block from file
	char block[server->block_size];
	read_from_file(server->remote_file, block, sizeof(block));

	// Get checksum of the new block
	checksum_t checksum;
	set_checksum(&checksum, (char*)&block, sizeof(block));

	while(!feof(server->remote_file)){
		int i = 0;
		int found_index = 0;
		bool found = false;
		while(i < server->checksum_list.size && !found){
			int i_element = list_get(&server->checksum_list, i);
			if(checksum.checksum == i_element){
				found = true;
				found_index = i;
			}
			i++;
		}
		if (!found){
			checksum_not_found((char*)&block, &window_out_bytes, server, &checksum);
		}else{
			if (window_out_bytes.size > 0){
				send_windowed_bytes(&window_out_bytes, server, skt);
				list_init(&window_out_bytes);
			}
			send_found_block_number(skt, found_index);
			read_from_file(server->remote_file, (char*)&block, sizeof(block));
			set_checksum(&checksum, (char*)&block, sizeof(block));
		}
	}
	// If there are remaining windowed bytes send them
	if (window_out_bytes.size > 0){
		send_windowed_bytes(&window_out_bytes, server, skt);
	}

	send_eof(skt);

	return EXIT_SUCCESS;
}

int receive_remote_filename(socket_t* skt, server_t* server){
	int filename_length;
	int block_size;

	socket_receive(skt, (char*)&filename_length, sizeof(int));

	char *name = malloc(filename_length + 1);
	socket_receive(skt, name, filename_length);
	name[filename_length] = 0;

	socket_receive(skt, (char*)&block_size, sizeof(int));

	server->block_size = block_size;

	// Open remote file here and assign to server_t
	server->remote_file = fopen(name, "r");

	free(name);

	return EXIT_SUCCESS;
}

int receive_checksum_list(socket_t* skt, unsigned int block_size,
		server_t* server){
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

int checksum_not_found(char* block, list_t* window_out_bytes, server_t* server,
		checksum_t* checksum){
	char byte_to_window = block[0];
	list_append(window_out_bytes, byte_to_window);
	fseek(server->remote_file, WINDOW_BYTE_DISPLACEMENT, SEEK_CUR);
	read_from_file(server->remote_file, block, server->block_size);
	char rolling_buffer[server->block_size + 1];
	memset(rolling_buffer, 0, sizeof(rolling_buffer));
	rolling_buffer[0] = byte_to_window;
	strncat(rolling_buffer, block, strlen(block));
	checksum_t old_checksum;
	old_checksum = *checksum;
	rolling_checksum(checksum, &old_checksum, (char*)&rolling_buffer +1,
			server->block_size);
	return EXIT_SUCCESS;
}

int send_windowed_bytes(list_t* window_out_bytes, server_t* server,
		socket_t* skt){
	char buffer_to_send[window_out_bytes->size];
	memset(buffer_to_send, 0, sizeof(buffer_to_send));
	for (int i = 0; i < window_out_bytes->size; ++i) {
		char i_element = list_get(window_out_bytes, i);
		strncat(buffer_to_send, &i_element, sizeof(i_element));
	}
	char new_bytes_indicator = NEW_BYTES_INDICATOR;
	socket_send(skt, (char*)&new_bytes_indicator, sizeof(new_bytes_indicator));

	// Send 4 bytes with the length of the new bytes
	int new_bytes_size = sizeof(buffer_to_send);
	socket_send(skt, (char*)&new_bytes_size, sizeof(new_bytes_size));

	// Send the actual bytes
	socket_send(skt, buffer_to_send, sizeof(buffer_to_send));
	list_free(window_out_bytes);
	return EXIT_SUCCESS;
}

int send_found_block_number(socket_t* skt, unsigned int index){
	char block_found_indicator = BLOCK_FOUND_INDICATOR;
	socket_send(skt, (char*)&block_found_indicator, sizeof(block_found_indicator));
	int block_number = index;
	socket_send(skt, (char*)&block_number, sizeof(block_number));
	return EXIT_SUCCESS;
}

int send_eof(socket_t* skt){
	char eof_indicator = EOF_INDICATOR;
	socket_send(skt, (char*)&eof_indicator, sizeof(eof_indicator));
	return EXIT_SUCCESS;
}
