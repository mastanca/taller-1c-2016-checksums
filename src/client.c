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
	socket_t skt;
	client.skt = skt;
	socket_init(&client.skt, hostname, port);
	printf("%s \n", "Socket created!");
	socket_connect(&client.skt);
	printf("%s \n", "Socket connected!");
	char input[100];
	char buffer[100];
	while (strcmp(input, "@exit\n") != 0){
		memset(&input, 0, sizeof(input));
		fgets(input, sizeof(input), stdin);
		socket_send(&client.skt, input, sizeof(input));
		socket_receive(&client.skt, buffer, sizeof(buffer));
		printf("%s", buffer);
	}
	socket_destroy(&client.skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

int send_remote_filename(socket_t* skt, char* filename){
	char *buffer = malloc( strlen(filename) + 8 );
	((int *)buffer)[0] = strlen( filename );
	strncat( buffer + 4, filename, strlen( filename ) );
	((int *)(buffer + 4 + strlen( filename )))[0] = 4;

	socket_send(skt->fd, buffer, strlen( filename ) + 8 );
	for ( int i = 0; i < stlren( filename ) + 8; i++ )
		printf( "%x ", buffer[ i ] );

	free( buffer );
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
