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

// Initiates checksum
int checksum_init(checksum_t* checksum);
// Frees checksum resources
int checksum_destroy(checksum_t* checksum);
// Calculates checksum value of input and sets it to checksum member
int checksum_set(checksum_t* checksum, char* input, size_t size);
// Given old checksum, calculates rolling checksum of buffer
// and stores it in new_checksum
int checksum_rolling(checksum_t* new_checksum, checksum_t* old_checksum,
		char* buffer, size_t size);
ulong get_checksum(checksum_t* checksum);

#endif /* SRC_LIB_CHECKSUM_H_ */
