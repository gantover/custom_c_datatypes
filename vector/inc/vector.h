#ifndef VECTOR_H
#define VECTOR_H

#define VEC_STATIC 0
#define VEC_DYNAMIC 1

#define VEC_OUT_OF_RANGE 10
#define VEC_WRONG_TYPE 11

typedef int (*filter_func) (void*);
typedef void (*map_func) (void*);
typedef int (*comp_func) (void*, void*);

typedef struct sVector vector;

struct sVector {
	void** items;
	int total;
	int capacity;
	int type;
	void (*push) (vector*, void*);
	void* (*get) (vector*, int);
	void (*rm) (vector*, int);
	void (*del) (vector*);
	void (*quicksort) (vector*, comp_func);
	vector* (*filter) (vector*, filter_func);
	vector* (*map) (vector*, map_func);
};

vector new_vector(unsigned int default_capacity, int type);

#endif // !VECTOR_H
