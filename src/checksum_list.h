/*
 * checksum_list.h
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#ifndef SRC_CHECKSUM_LIST_H_
#define SRC_CHECKSUM_LIST_H_

#define LIST_INITIAL_CAPACITY 100

typedef struct checksum_list {
	int size;      // slots used so far
	int capacity;  // total available slots
	int *data;     // array of integers we're storing
} checksum_list_t;

void checksum_list_init(checksum_list_t *checksum_list);

void checksum_list_append(checksum_list_t *checksum_list, int value);

int checksum_list_get(checksum_list_t *checksum_list, int index);

void checksum_list_set(checksum_list_t *checksum_list, int index, int value);

void checksum_list_double_capacity_if_full(checksum_list_t *checksum_list);

void checksum_list_free(checksum_list_t *checksum_list);

#endif /* SRC_CHECKSUM_LIST_H_ */
