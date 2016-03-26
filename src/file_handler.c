/*
 * file_handler.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

 #include "file_handler.h"
 #include <stdlib.h>
 
 // Reads block_size chars from file and return result in buffer
 int read_from_file(FILE* file, char* buffer, size_t block_size){
 	char tmp_buffer[block_size + 1];
 	memset(tmp_buffer, 0, sizeof(tmp_buffer));
 	if (!feof(file)){
 		if (fread(tmp_buffer, 1, block_size, file) != 0){
 			if (strlen(tmp_buffer) <= block_size)
 				strncpy(buffer, tmp_buffer, sizeof(tmp_buffer));
 		}
 	}
 	return EXIT_SUCCESS;
 }
