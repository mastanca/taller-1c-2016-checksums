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

	list_t list;
	server.checksum_list= list;
	list_init(&server.checksum_list);

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
//	printf("Printing list \n");
//	for (int var = 0; var < server.list.size; ++var) {
//		int temp = list_get(&server.list,var);
//		printf("%i \n", temp);
//	}
//	play_with_socket(server, &client_skt);

	if (server.remote_file != NULL)
		start_comparison_sequence(&server, &client_skt);

	socket_destroy(&client_skt);
	printf("%s \n", "Client destroyed!");
	socket_destroy(server.skt);
	printf("%s \n", "Socket destroyed!");
	fclose(server.remote_file);
	// Free checksum list
	list_free(&server.checksum_list);
	return 0;
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
	printf("Remote checksum of %s: %lx \n", block, checksum.checksum);

	while(!feof(server->remote_file)){
		int i = 0;
		int found_index = 0;
		bool found = false;
		while(i < server->checksum_list.size && !found ){
			int i_element = list_get(&server->checksum_list, i);
			if(checksum.checksum == i_element){
				found = true;
				found_index = i;
				printf("Checksum found in list \n");
			}
			i++;
		}
		if (!found){
			char byte_to_window = block[0];
			list_append(&window_out_bytes, byte_to_window);
			printf("Saved %c into windowed bytes\n", (char)byte_to_window);
			fseek(server->remote_file, WINDOW_BYTE_DISPLACEMENT, SEEK_CUR);
			read_from_file(server->remote_file, (char*)&block, sizeof(block));
			char rolling_buffer[server->block_size + 1];
			memset(rolling_buffer, 0, sizeof(rolling_buffer));
			rolling_buffer[0] = byte_to_window;
			strcat(rolling_buffer, block);
			checksum_t old_checksum;
			old_checksum = checksum;
			printf("Rolling buffer: %s\n", rolling_buffer);
			rolling_checksum(&checksum, &old_checksum, (char*)&rolling_buffer +1, server->block_size);
			printf("Remote rolling checksum of %s: %lx \n", block, checksum.checksum);
		}else{
			if (window_out_bytes.size > 0){
				for (int i = 0; i < window_out_bytes.size; ++i) {
					char i_element = list_get(&window_out_bytes, i);
					printf("Sending windowed %c \n", i_element);
				}
				list_free(&window_out_bytes);
				list_init(&window_out_bytes);
			}
			printf("Sending block number %i \n", found_index);
			read_from_file(server->remote_file, (char*)&block, sizeof(block));
			set_checksum(&checksum, (char*)&block, sizeof(block));
		}

	}
	// If there are remaining windowed bytes send them
	if (window_out_bytes.size > 0){
		for (int i = 0; i < window_out_bytes.size; ++i) {
			char i_element = list_get(&window_out_bytes, i);
			printf("Sending windowed %c \n", i_element);
		}
		list_free(&window_out_bytes);
	}

	// SEND END OF FILE HERE

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
	server->remote_file = fopen(name, "r");

	free(name);

	return 0;
}

int receive_checksum_list(socket_t* skt, size_t block_size, server_t* server){
	printf("%s \n", "Receiving checksum list");
	int code;
	int checksum;
	while ( code != END_OF_LIST){
		socket_receive(skt, (char*)&code, sizeof(code));
		code = ntohl(code);
		printf("Received code %i \n", code);
		if (CHECKSUM_INDICATOR == code){
			socket_receive(skt, (char*)&checksum, sizeof(checksum));
			checksum = ntohl(checksum);
			printf("Received checksum %i, of size %lx \n", checksum, sizeof(checksum));
			list_append(&(server->checksum_list), checksum);
		}
	}
	return 0;
}
