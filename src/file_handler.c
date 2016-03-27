/*
 * file_handler.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

 #include "file_handler.h"

 // Reads block_size chars from file and return result in buffer
 int read_from_file(FILE* file, char* buffer, size_t block_size, bool* read_something){
	 *read_something = false;
 	char tmp_buffer[block_size + 1];
 	memset(tmp_buffer, 0 , sizeof(tmp_buffer));
 	if (!feof(file)){
 		int read_bytes = fread(tmp_buffer, 1, block_size, file);
 		if (read_bytes != 0){
 			if (strlen(tmp_buffer) <= block_size){
 				strncpy(buffer, tmp_buffer, sizeof(tmp_buffer));
 				*read_something = true;
 			}
 		}
 	}
 	return EXIT_SUCCESS;
 }
