#ifndef LIST_H
#define LIST_H

#include <stdlib.h>


typedef struct
{
	void** data;
	size_t size;
	size_t used;
} List;

List* LIST_Create(size_t start_size);
void LIST_PushBack(List *list, void *item);
void LIST_Clear(List *list);
void LIST_Free(List *list);

#endif