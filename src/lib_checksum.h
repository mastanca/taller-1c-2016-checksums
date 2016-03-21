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

typedef struct checksum{
	unsigned long checksum;
	unsigned long lower;
	unsigned long higher;
} checksum_t;

int set_checksum_value(checksum_t* checksum, char* input, size_t size);
int rolling_checksum(checksum_t* new_checksum, checksum_t* old_checksum, char old_byte, char new_byte, size_t size);

#endif /* SRC_LIB_CHECKSUM_H_ */
