/*
 * client.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "client.h"

#define BLOQUE 100

int client_execution(int argc, char* argv[]){
	char* hostname = argv[2];
	char* port = argv[3];
	socket_t skt;
	socket_init(&skt, hostname, port);
	printf("%s \n", "Socket created!");
	socket_connect(&skt);
	printf("%s \n", "Socket connected!");
	char input[100];
	char buffer[100];
	while (strcmp(input, "@exit\n") != 0){
		memset(&input, 0, sizeof(input));
		fgets(input, sizeof(input), stdin);
		socket_send(&skt, input, sizeof(input));
		socket_receive(&skt, buffer, sizeof(buffer));
		printf("%s", buffer);
	}
	socket_destroy(&skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}

int open_file(FILE* file, char* file_route, char* mode){
	file = fopen(file_route, mode);
	if (file == NULL)
		return 1;
	procesar(file);
	return 0;
}

char *bloque(char *buffer, int bloques_leidos) {
	return (char *) &(buffer[bloques_leidos*BLOQUE]);
}

int leer(char *buffer, int bloques_leidos, FILE *descriptor, int *bytes_leidos) {
	int leidos_ahora = 0;
	leidos_ahora = fread(bloque(buffer, bloques_leidos++), 1, BLOQUE, descriptor);
	*bytes_leidos += leidos_ahora;
	return leidos_ahora;
}

int leer_de_archivo(FILE *descriptor, char *buffer) {
	int bytes_leidos = 0;
	int bloques_leidos = 0;
	while(leer(buffer, bloques_leidos++, descriptor, (int *)&bytes_leidos) == BLOQUE) {
		buffer = realloc(buffer, BLOQUE*(bloques_leidos + 1));
	}
	return bytes_leidos;
}

char *cargar_contenido(FILE *descriptor, int *bytes_leidos) {

	char *buffer = malloc(BLOQUE);

	if(descriptor != NULL)
		*bytes_leidos = leer_de_archivo(descriptor, buffer);

	return buffer;
}

void liberar_memoria(char *buffer) {
	if(buffer != NULL)
		free(buffer);
}

void procesar(FILE *entrada) {
	int bytes_leidos = 0;
	char *contenido = cargar_contenido(entrada, (int *)&bytes_leidos);
	if(bytes_leidos != 0)
		printf("%s \n", contenido);
	liberar_memoria(contenido);
}
