#include "./inc/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

clock_t start, end;

void triple_map(void* item) {
	int* elem = (int*)item;
	*elem = *elem * 3;
}

void print_map(void* item) {
	printf("%i\n", *(int*)item);
}

int even_filter(void *item) {
	if (*(int*)item % 2 == 0) {
		return true;
	}
	return false;
};

int* add_int(int *i) {
	int *num = malloc(sizeof(int));
	*num = *i;
	return num;
}

double test_vec(void) {
	start = clock();
	vector vec = new_vector(4096*64, VEC_DYNAMIC);
	for (int i = 0; i < vec.capacity; i++) {
		int* num = malloc(sizeof(int));
		*num = i;
		vec.push(&vec, num);
	}
	for (int i = 0; i < vec.total; i++) {
		vec.rm(&vec, -1);
	}
	// vec.map(&vec, print_map);
	// vec.filter(&vec, even_filter);
	end = clock();
	vec.del(&vec);
	double duration = ((double)end - start)/CLOCKS_PER_SEC;
	return duration;
}

int comp_le(void* i, void* j) {
	int int_i = *(int*)i;
	int int_j = *(int*)j;
	if (int_i <= int_j) {
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	// double duration = 0;
	// for (int i = 0; i < 1000; i++) {
	// 	duration += test_vec();
	// }
	// duration/=1000;
	// printf("duration : %f\n", duration);
	
	vector v = new_vector(8, VEC_DYNAMIC);

	for (int i = 0; i < 8; i++) {
		int* num = malloc(sizeof(int));
		// *num = 8 - i;
		*num = rand() % 50;
		v.push(&v, num);
	}
	v.map(&v, print_map);
	printf("quicksorting\n");
	v.quicksort(&v, comp_le);
	v.map(&v, print_map);


	return EXIT_SUCCESS;
}
