/*
 * checksum.h
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#ifndef SRC_LIB_CHECKSUM_H_
#define SRC_LIB_CHECKSUM_H_

#include <stddef.h>
#define M 0x00010000

typedef unsigned long ulong;

typedef struct checksum{
	ulong checksum;
	ulong lower;
	ulong higher;
} checksum_t;

int set_checksum(checksum_t* checksum, char* input, size_t size);
int rolling_checksum(checksum_t* new_checksum, checksum_t* old_checksum,
		char* buffer, size_t size);

#endif /* SRC_LIB_CHECKSUM_H_ */
