/*
 * file_handler.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#include "file_handler.h"

// Reads block_size chars from file and return result in buffer
int read_from_file(FILE* file, char* buffer, size_t block_size){
	if (!feof(file)){
		fgets(buffer, block_size + 1, file);
	 }
	return 0;
}


