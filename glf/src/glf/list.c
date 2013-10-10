#include "glf/list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct glf_list_node
{
	void                 *data;
	struct glf_list_node *next;
	struct glf_list_node *prev;
} glf_list_node;

typedef struct glf_list
{
	glf_list_node *head;
	glf_list_node *tail;
	glf_list_node *iter; /**< Current */
} glf_list;

glf_list *glf_list_create()
{
	glf_list *newl = malloc(sizeof(glf_list));

	if(newl)
	{
		memset(newl, NULL, sizeof(glf_list));
	}

	return newl;
}

void glf_list_destroy(glf_list *list, DestroyFunction fn)
{
	assert(list != NULL);

	glf_list_node *n = list->head;

	while(n)
	{
		fn(n->data);
		glf_list_node *next = n->next;
		free(n);
		n = next;
	}

	free(list);
}

int glf_list_is_empty(glf_list *list)
{
	assert(list);
	return list->head == NULL;
}

int glf_list_append(glf_list *list, void *item)
{
	glf_list_node *newn;

	assert(list);

	/* Sanity checks */
	if(!item)
	{
		return -1;
	}

	/* Create node */
	newn = malloc(sizeof(glf_list_node));

	if(!newn)
	{
		return -2;
	}

	newn->data = item;

	/* Modify list and last node */
	if(!list->tail) /* Empty? */
	{
		newn->next = newn->prev = NULL;
		list->head = list->tail = newn;
	}
	else
	{
		newn->prev = list->tail;
		newn->next = NULL;
		list->tail->next = newn;
		list->tail = newn;
	}

	list->iter = newn;
	return 0;
}

int glf_list_insert(glf_list *list, void *item)
{
	glf_list_node *newn;

	assert(list != NULL);

	/* Sanity checks */
	if(!item)
	{
		return -1;
	}

	/* Create node */
	newn = malloc(sizeof(glf_list_node));

	if(!newn)
	{
		return -2;
	}

	newn->data = item;

	/* Reorganize */
	if(list->head == NULL) /* emtpy */
	{
		newn->next = newn->prev = NULL;
		list->head = list->tail = newn;
	}
	else if(list->iter == list->head) /* in front of the list */
	{
		newn->prev = NULL;
		newn->next = list->head;

		list->head->prev = newn;
		list->head = newn;
	}
	else
	{
		newn->prev = list->iter->prev;
		newn->next = list->iter;

		list->iter->prev->next = newn;

		list->iter->prev = newn;
	}

	list->iter = newn;
	return 0;
}

void* glf_list_remove(glf_list *list)
{
	assert(list != NULL);

	if(linkedListIsEmpty(list))
	{
		return NULL;
	}

	/* reorganize */
	glf_list_node *prev = list->iter->prev;
	glf_list_node *next = list->iter->next;

	if(prev)
	{
		prev->next = next;
	}
	else
	{
		list->head = next;
	}

	if(next)
	{
		next->prev = prev;
	}
	else
	{
		list->tail = prev;
	}

	// free
	void *data = list->iter->data;
	free(list->iter);

	// set iter
	if(next)
	{
		list->iter = next;
	}
	else
	{
		list->iter = prev;
	}

   	return data;
}

void* glf_list_get_current(glf_list* list)
{
	assert(list != NULL);
	return (glf_list_is_empty(list)) ? NULL : list->iter->data;
}

void* glf_list_get_first(glf_list *list)
{
	if(glf_list_is_empty(list))
	{
		return NULL;
	}

	list->iter = list->head;
	return list->iter->data;
}

void *glf_list_get_last(glf_list *list)
{
	if(glf_list_is_empty(list))
	{
		return NULL;
	}

	list->iter = list->tail;
	return list->iter->data;
}

void *glf_list_get_next(glf_list *list)
{
	/* if empty or tail */
	if(glf_list_is_empty(list) || list->iter->next == NULL) 
	{
		return NULL;
	}

	list->iter = list->iter->next;
	return list->iter->data;
}

void *glf_list_get_prev(glf_list *list)
{
	/* if empty or head */
	if(glf_list_is_empty(list) || list->iter->prev == NULL) 
	{
		return NULL;
	}

	list->iter = list->iter->prev;
	return list->iter->data;
}

static void glf_list_swap(glf_list_node *a, glf_list_node *b)
{
	void *tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}

// Recursive function
static void qsort_(glf_list_node *begin, glf_list_node *end, CompareFunction fn,
	void *usr_info)
{
	if(end == begin)
	{
		return;
	}

	glf_list_node *l = begin;
	glf_list_node *r = end;
	void *p = begin->data;

	while(l != r)
	{
		if(fn(p, l->data, usr_info) <= 0)
		{
			l = l->next;
		}
		else if(fn(p, r->data, usr_info) >= 0)
		{
			r = r->prev;
		}
		else
		{
			swap(l, r);
		}
	}

	if(fn(p, l->data, usr_info) < 0)
	{
		swap(l, begin);
		l = l->prev;
	}
	else
	{
		l = l->prev;
		swap(l, begin);
	}

	qsort_(begin, l, fn, usr_info);
	qsort_(r, end, fn, usr_info);
}

// Sort the linked list
// fn is a function used to compare two list items
// usr_info is a value passed to every call of fn
int linkedListSort(glf_list * list, CompareFunction fn, void *usr_info)
{
	assert(list != NULL);

	if(linkedListIsEmpty(list) || list->head == list->tail)
	{
		return 0;
	}

	qsort_(list->head, list->tail, fn, usr_info);
	return 0;
}
