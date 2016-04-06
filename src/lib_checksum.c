/*
 * checksum.c
 *
 *  Created on: Mar 20, 2016
 *      Author: mastanca
 */

#include "lib_checksum.h"
#include <stdio.h>
#include <stdlib.h>

int checksum_init(checksum_t* checksum){
	checksum->lower = 0;
	checksum->higher = 0;
	checksum->checksum = 0;
	return EXIT_SUCCESS;
}

static int set_checksum_result(checksum_t* checksum){
	checksum->lower %= M;
	checksum->higher %= M;
	checksum->checksum = checksum->lower + checksum->higher*M;
	return EXIT_SUCCESS;
}

// Stores the resulting checksum in checksum arg
int checksum_set(checksum_t* checksum, char* input, size_t size){

	for (unsigned int i = 0; i < size; ++i) {
		checksum->lower += input[i];
		checksum->higher += ((size-i)*input[i]);
	}

	set_checksum_result(checksum);

	return EXIT_SUCCESS;
}

// Rolling checksum assumes buffer is contiguous in memory
int checksum_rolling(checksum_t* new_checksum, checksum_t* old_checksum,
		char* buffer, size_t size){
	checksum_init(new_checksum);

	new_checksum->lower = ((old_checksum->lower - (ulong)buffer[-1] +
			(ulong)buffer[size-1])) % M;
	new_checksum->higher = old_checksum->higher - (size * (ulong)buffer[-1]) +
			new_checksum->lower;

	set_checksum_result(new_checksum);

	return EXIT_SUCCESS;
}

ulong get_checksum(checksum_t* checksum){
	return checksum->checksum;
}

