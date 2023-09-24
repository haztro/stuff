#include "data/list.h"

static void LIST_Init(List *list, size_t start_size);

List* LIST_Create(size_t start_size)
{
	List *list = (List *) malloc(sizeof(List));
	LIST_Init(list, start_size);

	return list;
}

static void LIST_Init(List *list, size_t start_size)
{
	list->data = (void *) malloc(start_size * sizeof(void *));
	list->used = 0;
	list->size = start_size;
}

void LIST_PushBack(List *list, void *item)
{
	if (list->used == list->size)
	{
		list->size *= 2;
		list->data = (void *) realloc(list->data, 
										list->size * sizeof(void *));
	}
	list->data[list->used++] = item;
}

void LIST_Clear(List *list)
{
	list->used = 0;
}

void LIST_Free(List *list)
{
	free(list->data);
	list->data = NULL;
	list->used = 0;
	list->size = 0;
}