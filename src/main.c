/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <string.h>
#include "client.h"
#include "server.h"
#include "checksum.h"

int main(int argc, char *argv[]){
//	char* execution_type = argv[1];
//	if (strcmp(execution_type, "client") == 0){
//		return client_execution(argc, argv);
//
//	}else if(strcmp(execution_type, "server") == 0){
//		return server_execution(argc, argv);
//	} else {
//		return 1;
//	}
	printf("%i\n", checksum(argv, 4));
	return 0;
}
