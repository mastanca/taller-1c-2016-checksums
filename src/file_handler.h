/*
 * file_handler.h
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#ifndef SRC_FILE_HANDLER_H_
#define SRC_FILE_HANDLER_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int read_from_file(FILE* file, char* buffer, size_t block_size, bool* read_something);


#endif /* SRC_FILE_HANDLER_H_ */
