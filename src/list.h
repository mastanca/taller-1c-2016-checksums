/*
 * list.h
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#define LIST_INITIAL_CAPACITY 100

typedef struct list {
	int size;      // slots used so far
	int capacity;  // total available slots
	int *data;     // array of integers we're storing
} list_t;

void list_init(list_t *list);

void list_append(list_t *list, int value);

int list_get(list_t *list, int index);

void list_set(list_t *list, int index, int value);

void list_double_capacity_if_full(list_t *list);

void list_free(list_t *list);

#endif /* SRC_LIST_H_ */
