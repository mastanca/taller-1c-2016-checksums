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

#define CLIENT "client"
#define SERVER "server"


int main(int argc, char *argv[]){
//	char* execution_type = argv[1];
//	if (strcmp(execution_type, CLIENT) == 0){
//		return client_execution(argc, argv);
//
//	}else if(strcmp(execution_type, SERVER) == 0){
//		return server_execution(argc, argv);
//	} else {
//		return 1;
//	}
	checksum_t checksum;
	char* test_checksum = "ABCDEFG";
	set_checksum(&checksum, test_checksum, 3);
	printf("%lu\n" , checksum.checksum);

	char* test_new_checksum = "ABCDEFG";

	checksum_t new_old_checksum;
	set_checksum(&new_old_checksum, test_new_checksum + 1, 3);
	printf("%lu\n" , new_old_checksum.checksum);

	checksum_t new_checksum;
	rolling_checksum(&new_checksum, &checksum, test_checksum + 1 , 3);
	printf("%lu\n" , new_checksum.checksum);
	return 0;
}
