/*
 * checksum.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "lib_checksum.h"

static int checksum_init(checksum_t* checksum){
	checksum->lower = 0;
	checksum->higher = 0;
	checksum->checksum = 0;
	return 0;
}

static int set_checksum_result(checksum_t* checksum){
	checksum->lower %= M;
	checksum->higher %= M;
	checksum->checksum = checksum->lower + checksum->higher*M;
	return 0;
}

int set_checksum_value(checksum_t* checksum, char* input, size_t size){
	checksum_init(checksum);

	for (int i = 0; i < size; ++i) {
		checksum->lower += input[i];
		checksum->higher += ((size-i)*input[i]);
	}

	set_checksum_result(checksum);

	return 0;
}

int rolling_checksum(checksum_t* new_checksum, checksum_t* old_checksum, char old_byte, char new_byte, size_t size){
	checksum_init(new_checksum);

	new_checksum->lower = old_checksum->lower - old_byte + new_byte;
	new_checksum->higher = old_checksum->higher - size*old_byte + new_checksum->lower;

	set_checksum_result(new_checksum);

	return 0;
}

