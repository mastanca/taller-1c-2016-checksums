/*
 * file_handler.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#include "file_handler.h"

// Reads block_size chars from file and return result in buffer
int read_from_file(FILE* file, char* buffer, size_t block_size){
	char tmp_buffer[block_size + 1];
	if (!feof(file))
		fgets(tmp_buffer, block_size + 1, file);
	if (strlen(tmp_buffer) == block_size)
		strcpy(buffer, tmp_buffer);
	return 0;
}


