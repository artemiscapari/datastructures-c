#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
struct stack {

  int stack_array[STACK_SIZE];
  int size;
  int push_amount;
  int pop_amount;
  int max_stack_size;
};

struct stack *stack_init() {
  struct stack *s = malloc(sizeof(struct stack));
  s->size = 0;
  s->push_amount = 0;
  s->pop_amount = 0;
  s->max_stack_size = 0;
  return s;
}

void stack_cleanup(struct stack *s) {
  fprintf(stderr, "stats %d %d %d",s->push_amount, s->pop_amount, s->max_stack_size);
  free(s);
}

int stack_push(struct stack *s, int c) {
  if(s->size >= STACK_SIZE){
    return 1;
  }
  s->stack_array[s->size] = c;
  s->size += 1;
  s->push_amount += 1;
  if (s->size > s->max_stack_size) {
    s->max_stack_size = s->size;
  }
  return 0;
}

int stack_pop(struct stack *s) {
  if(s->size == 0){
    return -1;
  }
  s->size -= 1;
  s->pop_amount += 1;
  int popped = s->stack_array[s->size];
  return popped;
}

int stack_peek(struct stack *s) {
  if(s->size == 0){
    return -1;
  }
  return s->stack_array[s->size - 1];
}

int stack_empty(struct stack *s) {
  return s->size == 0;
}
