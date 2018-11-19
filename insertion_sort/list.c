#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
  int data;
  struct node *next;
} node;

struct list {
  struct node *head;
  struct node *tail;
  int length;
};

struct list *list_init() {
  struct list *l = malloc(sizeof(struct list));
  if(l == NULL) return NULL;
  l->length = 0;
  l->head = NULL;
  l->tail = NULL;
  return l;
}

int list_cleanup(struct list *l) {
  struct node* head = l->head;
  struct node* tmp;
  while(head != NULL){
    tmp = head;
    head = head->next;
    list_free_node(tmp);
  }
  free(l);
  return 0;
}

struct node *list_new_node(int num) {
  struct node *n = malloc(sizeof(struct node));
  if(n == NULL) return NULL;
  n->data = num;
  n->next = NULL;
  return n;
}

int list_add(struct list *l, int num) {
  struct node *n = list_new_node(num);
  if(n == NULL) return 1;
  if (l->length == 0) {
    l->tail = n;
  }
  if (l->head != NULL) {
    n->next = l->head;
  }
  l->head = n;
  l->length++;
  return 0;
}

int list_add_back(struct list *l, int num) {
  struct node *n = list_new_node(num);
  if(n == NULL) return 1;
  if (l->length == 0) {
    l->head = n;
  }
  if (l->tail != NULL) {
    l->tail->next = n;
  }
  l->tail = n;
  l->length++;
  return 0;
}

struct node *list_head(struct list *l) {
  return l->head;
}

int list_length(struct list *l) { return l->length; }

int list_node_data(struct node *n) { return n->data; }

struct node *list_next(struct node *n) {
  return n->next;
}

struct node *list_prev(struct list *l, struct node *n) {
  for (node *cur = l->head; cur; cur = cur->next) {
    if (cur->next == n) {
      return cur;
    }
  }
  return NULL;
}

int list_unlink_node(struct list *l, struct node *n) {
  if (l == NULL || n == NULL) {
    return 1;
  }

  node *prev = list_prev(l, n);
  // If the element is the first one in the list, only change the head pointer
  // to point to its next.
  if (prev == NULL) {
    l->head = n->next;
  }
  // If the element is the last one in the list, change the tail pointer to
  // point to previous.
  if (n->next == NULL) {
    l->tail = prev;
    // Also fix new last element's pointer to point to NULL.
    if (prev != NULL) {
      prev->next = NULL;
    }
  }
  // Else if this is an element that is nor the first, nor the last, fix the
  // pointers of the previous and next node.
  if (prev != NULL && n->next != NULL) {
    prev->next = n->next;
  }

  l->length--;
  return 0;
}

void list_free_node(struct node *n) {
  if(n != NULL){
    free(n);
  }
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
  bool found = false;
  for (node *cur = l->head; cur; cur = cur->next) {
    if (cur == n) {
      return 1;
    } else if (cur == m) {
      found = true;
    }
  }
  if (!found) {
    return 1;
  }
  if (m->next == NULL) {
    l->tail = n;
  } else{
    n->next = m->next;
  }
  m->next = n;
  l->length++;
  return 0;
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
  bool found = false;
  for (node *cur = l->head; cur; cur = cur->next) {
    if (cur == n) {
      return 1;
    } else if (cur == m) {
      found = true;
    }
  }
  if (!found) {
    return 1;
  }
  node *prev = list_prev(l, m);
  if (prev == NULL) {
    l->head = n;
    n->next = m;
  } else {
    prev->next = n;
    n->next = m;
  }
  l->length++;
  return 0;
}
