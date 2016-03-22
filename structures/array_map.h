#ifndef VIOLET_ARRAY_MAP_H
#define VIOLET_ARRAY_MAP_H

#include "violet/structures/array.h"

typedef struct array_map
{
	array pairs;
	u32 key_size;
} array_map;

void array_map_init(array_map * m, u32 key_size, u32 val_size);
void array_map_copy(array_map * dest, const array_map * src);
void array_map_destroy(array_map * m);
void array_map_insert(array_map * m, const void * key, const void * val);
void * array_map_insert_null(array_map * m, const void * key);
u32 array_map_size(const array_map * m);
void * array_map_get(const array_map * m, const void * key);
b8 array_map_has(const array_map * m, const void * key);

typedef struct array_map_iter
{
	void * key;
	void * val;
	u32 next_idx;
} array_map_iter;

b8 array_map_iterate(const array_map * m, array_map_iter * iter);
void array_map_remove_iter(array_map * m, array_map_iter * iter);
void array_map_iter_reset(array_map_iter * iter);

#endif

