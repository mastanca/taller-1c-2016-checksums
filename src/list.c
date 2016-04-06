/*
 * list.c
 *
 *  Created on: Mar 24, 2016
 *      Author: mastanca
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void list_init(list_t *list) {
	list->size = 0;
	list->capacity = LIST_INITIAL_CAPACITY;
	list->data = malloc(sizeof(int) * list->capacity);
}

void list_append(list_t *list, int value) {
	list_double_capacity_if_full(list);
	list->data[list->size++] = value;
}

// Return the block at the given index
int list_get(list_t *list, int index) {
	if (index >= list->size || index < 0) {
		return EXIT_FAILURE;
	}
	return list->data[index];
}

void list_set(list_t *list, int index, int value) {
	while (index >= list->size) {
		list_append(list, 0);
	}

	list->data[index] = value;
}

void list_double_capacity_if_full(list_t *list) {
	if (list->size >= list->capacity) {
		list->capacity *= 2;
		list->data = realloc(list->data, sizeof(int) * list->capacity);
	}
}

void list_free(list_t *list) {
	free(list->data);
}

int list_get_size(list_t* list){
	return list->size;
}

