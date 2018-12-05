#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

int swap(struct heap *h, int first_index, int second_index) {
  void *first = array_get(h->array, first_index);
  void *second = array_get(h->array, second_index);
  if (first == NULL || second == NULL) {
    return 1;
  }
  array_set(h->array, second_index, first);
  array_set(h->array, first_index, second);
  return 0;
}

int left(int index) {
  return index * 2 + 1;
}

int right(int index) {
  return index * 2 + 2;
}

int parent(int index) {
  return (index - 1) / 2;
}

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
  prioq *h = malloc(sizeof(prioq));
  if (h == NULL)
    return NULL;
  struct array *a = array_init(10);
  if (a == NULL) {
    free(h);
    return NULL;
  }
  h->array = a;
  h->compare = compare;
  return h;
}

struct heap *prioq_init(int (*compare)(const void *, const void *)) {
  return heap_init(compare);
}

long int prioq_size(struct heap *h) {
  return array_size(h->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
  if (h == NULL) {
    return 1;
  }
  array_cleanup(h->array, free_func);
  free(h);
  return 0;
}

int prioq_cleanup(prioq *h, void free_func(void *)) {
  return heap_cleanup(h, free_func);
}

static int heap_insert(struct heap *h, void *p) {
  int index = (int)prioq_size(h);
  int set = array_append(h->array, p);
  if (set == -1) {
    return -1;
  }

  if (prioq_size(h) == 1) {
    return 0;
  }

  while (index > 0) {
    int parent_index = parent(index);
    void *parent = array_get(h->array, parent_index);
    void *current = array_get(h->array, index);
    int res = h->compare(current, parent);
    if (res < 0) {
      swap(h, index, parent_index);
      index = parent_index;
    } else {
      // Else if the parent is higher then simply break the while loop.
      break;
    }
  }
  return 0;
}

int prioq_insert(prioq *h, void *p) { return heap_insert(h, p); }

static void *heap_pop(struct heap *h) {
  int size = (int)prioq_size(h);
  if (size == 0) {
    return NULL;
  }
  if (swap(h, 0, size - 1) == 1) {
    return NULL;
  }
  void *val = array_pop(h->array);
  int index = 0;
  // If the queue is either empty or contains a single element there is no need
  // for sorting.
  if (prioq_size(h) == 1) {
    return val;
  }
  void *current_val = array_get(h->array, index);
  void *right_val = array_get(h->array, right(index));
  void *left_val = array_get(h->array, left(index));
  // Due to the fact that there may be nodes with a single leaf, first check if
  // there is at least one leaf that can be compared against.
  while (current_val != NULL && (right_val != NULL || left_val != NULL)) {
    bool swap_left = false;
    bool swap_right = false;
    int swap_index;

    if (left_val != NULL) {
      swap_left = h->compare(current_val, left_val) > 0;
    }
    if (right_val != NULL) {
      swap_right = h->compare(current_val, right_val) > 0;
    }
    // If the current cannot be swaped with either of the children then break.
    if (!swap_left && !swap_right) {
      break;
    }
    // If both children can be swapped choose the lower one.
    if (swap_left && swap_right) {
      swap_index =
          h->compare(left_val, right_val) < 0 ? left(index) : right(index);
    } else {
      swap_index = swap_left ? left(index) : right(index);
    }
    swap(h, index, swap_index);
    index = swap_index;
    current_val = array_get(h->array, index);
    right_val = array_get(h->array, right(index));
    left_val = array_get(h->array, left(index));
  }

  return val;
}

void *prioq_pop(prioq *h) { return heap_pop(h); }
