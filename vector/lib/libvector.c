#include <stdio.h>
#include <stdlib.h>
#include "../inc/vector.h"
#include <stdbool.h>

void catch_exit(int code) {
	switch (code) {
		case VEC_OUT_OF_RANGE: 
			printf("usage of a non valid index on a vector\n");
		break;
		case VEC_WRONG_TYPE:
			printf("wrong vector type, please chose between VEC_DYNAMIC and VEC_STATIC\n");
		break;
	}
	exit(EXIT_FAILURE);
}

void intercept_index(vector* self, int *index) {
	if (*index < 0 && abs(*index) <= self->total) {
		*index += self->total;
	} else if (*index >= self->total) {
		catch_exit(VEC_OUT_OF_RANGE);
	}
}

void vector_resize(vector* self, unsigned int new_capacity) {
	if (new_capacity >= self->total) {
		self->items = realloc(self->items, new_capacity);
		self->capacity = new_capacity;
	}
}

void vector_push(vector* self, void* item) {
	if (self->total == self->capacity) {
		vector_resize(self, self->capacity * 2);
	}
	self->items[self->total++] = item;
}

void* vector_get(vector* self, int index) {
	void* item = NULL;
	intercept_index(self, &index);
	item = self->items[index];
	return item;
}

void vector_remove(vector *self, int index) {
	intercept_index(self, &index);
	self->items[index] = NULL;
	for (int i = index; i < self->total-1; i++) {
		self->items[i] = self->items[i+1];
	}
	self->items[--self->total] = NULL;
	// the last item is at index total -1
	// the new total also has to be updated to total -1
	// we do both at the same time
	if (self->total * 2 >= self->capacity) {
		vector_resize(self, self->capacity/2);
	}
}

void vector_remove_and_free(vector *self, int index) {
	intercept_index(self, &index);
	free(self->items[index]);
	self->items[index] = NULL;
	for (int i = index; i < (self->total-1); i++) {
		self->items[i] = self->items[i+1];
	}
	self->items[--self->total] = NULL;
	// the last item is at index total -1
	// the new total also has to be updated to total -1
	// we do both at the same time
	if (self->total * 2 == self->capacity) {
		vector_resize(self, self->capacity/2);
	}
}

void vector_delete(vector *self) {
	free(self->items);
}

void vector_delete_free_elements(vector *self) {
	for (int i = 0; i < self->total; i++) {
		free(self->items[i]);
	}
	free(self->items);
}


vector clone(vector original) {
	void** original_items = original.items;

	void** new_items = malloc(sizeof(void*) * original.capacity);
	// copy
	for (int i = 0; i < original.total; i++) {
		new_items[i] = original_items[i];
	}
	original.items = new_items;

	return original;
}


vector* vector_filter(vector* self, filter_func f) {
	vector filtered = new_vector(self->capacity/2, self->type);
	for (int i = 0; i < self->total; i++) {
		if (f(self->items[i]) == true) {
			filtered.push(&filtered, self->items[i]);
		} 
	}
	vector_delete(self); 
	*self = filtered;
	return self;
}

vector* vector_filter_and_free(vector* self, filter_func f) {
	vector filtered = new_vector(self->capacity/2, VEC_DYNAMIC);
	for (int i = 0; i < self->total; i++) {
		if (f(self->items[i]) == true) {
			filtered.push(&filtered, self->items[i]);
		} else {
			free(self->items[i]);
		}
	}
	vector_delete(self);
	*self = filtered;
	return self;
}

vector* vector_map(vector* self, map_func f) {
	for (int i = 0; i < self->total; i++) {
		f(self->items[i]);
	}
	return self;
}


void swap(vector* self, int i, int j) {
	void* tmp = self->items[i];
	self->items[i] = self->items[j];
	self->items[j] = tmp;
}

int partition(vector *self, int start, int end, comp_func f) {
	int i = start - 1;
	void* x = self->items[end];
	
	for (int j = start; j < end; j++) {
		if (f(self->items[j], x)) {
			i = i + 1;
			swap(self,i,j);
		}
	}
	swap(self, i+1, end);
	return i+1;
}


void quicksort(vector* self, int start, int end, comp_func f) {
	if (start < end) {
		int part = partition(self, start, end, f);
		quicksort(self, start, part-1, f);
		quicksort(self, part+1, end, f);
	}
}

void quicksort_init(vector* self, comp_func f) {
	quicksort(self, 0, self->total-1, f);
}

vector new_vector(unsigned int default_capacity, int type) {
	vector self;
	if (type == VEC_DYNAMIC) {
		self.rm = vector_remove_and_free;
		self.del = vector_delete_free_elements;
		self.filter = vector_filter_and_free;
	} else if (type == VEC_STATIC) {
		self.rm = vector_remove;
		self.del = vector_delete;
		self.filter = vector_filter;
	} else {
		catch_exit(VEC_WRONG_TYPE);
	}
	void** items = malloc(sizeof(void*) * default_capacity);
	self.type = type;
	self.items = items;
	self.total = 0;
	self.capacity = default_capacity;
	self.push = vector_push;
	self.get = vector_get;
	self.map = vector_map;
	self.quicksort = quicksort_init;
	return self;
} 

