/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <string.h>
#include "client.h"
#include "lib_checksum.h"
#include "server.h"

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
	checksum_t checksum;
	char* test_checksum = "ABC";
	set_checksum_value(&checksum, test_checksum, 3);
	printf("%lu\n" , checksum.checksum);


	checksum_t new_checksum;
	rolling_checksum(&new_checksum, &checksum, 'A', 'D', 3);
	printf("%lu\n" , new_checksum.checksum);
	return 0;
}
