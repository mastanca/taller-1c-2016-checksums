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
	skt.fd = 125;
	printf("%d \n", skt.fd);
	return 0;
}
