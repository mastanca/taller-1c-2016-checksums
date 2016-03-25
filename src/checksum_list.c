/*
 * checksum_list.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#include <stdio.h>
#include <stdlib.h>
#include "checksum_list.h"

void checksum_list_init(checksum_list_t *checksum_list) {
	checksum_list->size = 0;
	checksum_list->capacity = LIST_INITIAL_CAPACITY;
	checksum_list->data = malloc(sizeof(int) * checksum_list->capacity);
}

void checksum_list_append(checksum_list_t *checksum_list, int value) {
	checksum_list_double_capacity_if_full(checksum_list);
	checksum_list->data[checksum_list->size++] = value;
}

// Return the block at the given index
int checksum_list_get(checksum_list_t *checksum_list, int index) {
	if (index >= checksum_list->size || index < 0) {
		printf("Index %d out of bounds for checksum_list of size %d\n", index,
				checksum_list->size);
		return -1;
	}
	return checksum_list->data[index];
}

void checksum_list_set(checksum_list_t *checksum_list, int index, int value) {
	while (index >= checksum_list->size) {
		checksum_list_append(checksum_list, 0);
	}

	checksum_list->data[index] = value;
}

void checksum_list_double_capacity_if_full(checksum_list_t *checksum_list) {
	if (checksum_list->size >= checksum_list->capacity) {
		checksum_list->capacity *= 2;
		checksum_list->data = realloc(checksum_list->data, sizeof(int) * checksum_list->capacity);
	}
}

void checksum_list_free(checksum_list_t *checksum_list) {
	free(checksum_list->data);
}
