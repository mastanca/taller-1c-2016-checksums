/*
 * client.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include "lib_socket.h"

typedef struct client{
	FILE* old_file;
}client_t;

int client_execution(int argc, char* argv[]);
int open_file(FILE* file, char* file_route, char* mode);
void procesar(FILE *entrada);
void liberar_memoria(char *buffer);
char *cargar_contenido(FILE *descriptor, int *bytes_leidos);
int leer_de_archivo(FILE *descriptor, char *buffer);
int leer(char *buffer, int bloques_leidos, FILE *descriptor, int *bytes_leidos);
char *bloque(char *buffer, int bloques_leidos);




#endif /* SRC_CLIENT_H_ */
