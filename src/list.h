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

// Initiate list
void list_init(list_t *list);

// Append value
void list_append(list_t *list, int value);

// Return the element at index
int list_get(list_t *list, int index);

// Set a value at position index
void list_set(list_t *list, int index, int value);

// Double de lists capacity
void list_double_capacity_if_full(list_t *list);

// Free the lists resources
void list_free(list_t *list);

// Return size of the list
int list_get_size(list_t* list);

#endif /* SRC_LIST_H_ */
