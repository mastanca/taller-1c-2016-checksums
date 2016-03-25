/*
 * main.c

 *
 *  Created on: Mar 10, 2016
 *      Author: mastanca
 */

#include <string.h>
#include "client.h"
//#include "lib_checksum.h"
#include "server.h"

#define CLIENT "client"
#define SERVER "server"


int main(int argc, char *argv[]){
// SERVER TESTING
	char* execution_type = argv[1];
	if (strcmp(execution_type, CLIENT) == 0){
		return client_execution(argc, argv);

	}else if(strcmp(execution_type, SERVER) == 0){
		return server_execution(argc, argv);
	} else {
		return 1;
	}

// CHECKSUM TESTING

//	checksum_t checksum;
//	char* test_checksum = "xaa";
//	set_checksum(&checksum, test_checksum, 2);
//	printf("%lu\n" , checksum.checksum);
//
//	char* test_new_checksum = "xaa";
//
//	checksum_t new_old_checksum;
//	set_checksum(&new_old_checksum, test_new_checksum + 1, 2);
//	printf("%04x\n" , (unsigned int)new_old_checksum.checksum);
//
//	checksum_t new_checksum;
//	rolling_checksum(&new_checksum, &checksum, test_checksum + 1 , 2);
//	printf("%04x\n" , (unsigned int)new_checksum.checksum);
//
//	FILE* file = fopen("remote", "r");;
//	char buffer[100];
//	memset(buffer, 0, sizeof(buffer));
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fseek(file, -1, SEEK_CUR);
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fseek(file, -1, SEEK_CUR);
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fseek(file, -1, SEEK_CUR);
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fread(&buffer, sizeof(char), 2, file);
//	printf("%s \n", buffer);
//	fclose(file);

	return 0;
}
