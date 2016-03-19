/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <stdio.h>
#include <string.h>

#include "lib_socket.h"

int main(int argc, char *argv[])
{
	socket_t skt;
	socket_init(&skt);
	printf("%s \n", "Socket created!");
	socket_bind(&skt);
	printf("%s \n", "Socket binded!");
	socket_destroy(&skt);
	printf("%s \n", "Socket destroyed!");
	return 0;
}
