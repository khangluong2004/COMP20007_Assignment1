/*
list.c

Implementations for helper functions for linked list construction and
manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include "list.h"
#include <stdlib.h>
#include <assert.h>

struct list {
  void *item;
  struct list *next;
};

struct list *newlist(void *item){
  struct list *head = (struct list *) malloc(sizeof(struct list));
  assert(head);
  head->item = item;
  head->next = NULL;
  return head;
}

struct list *prependList(struct list *list, void *item){
  struct list *head = (struct list *) malloc(sizeof(struct list));
  assert(head);
  head->item = item;
  head->next = list;
  return head;
}

void *peekHead(struct list *list){
  if(! list){
    return NULL;
  }
  return list->item;
}

void *deleteHead(struct list **list){
  void *item;
  struct list *next;
  if(! list || ! *list){
    return NULL;
  }
  /* Store values we're interested in before freeing list node. */
  item = (*list)->item;
  next = (*list)->next;
  free(*list);
  *list = next;
  return item;
}

void freeList(struct list *list){
  struct list *next;
  /* Iterate through list until the end of the list (NULL) is reached. */
  for(next = list; list != NULL; list = next){
    /* Store next pointer before we free list's space. */
    next = list->next;
    free(list);
  }
}
