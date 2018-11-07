#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

void to_rpn(struct stack *s, char *input);
bool has_presedence(char current, char verified);

void to_rpn(struct stack *s, char *input) {
  for (int i = 0; i < strlen(input); i++) {
    char c = input[i];
    if (isdigit(c) || isalpha(c)) {
      putchar(c);
      continue;
    } else if (c == '(') {
      stack_push(s, c);
    } else if (c == ')') {
      while (stack_peek(s) != '(') {
        putchar(stack_pop(s));
      }
      // Pop the stack to get rid off the left parenthesis.
      stack_pop(s);
    } else {
      if(stack_empty(s)){
        stack_push(s,c);
        continue;
      }
      while(!stack_empty(s) && stack_peek(s) != '(' && !has_presedence(c, stack_peek(s))){
        putchar(stack_pop(s));
      }
      stack_push(s, c);
    }
  }
  while (!stack_empty(s)) {
    putchar(stack_pop(s));
  }
}

bool has_presedence(char left, char right) {
  if (left == '~' && right != '~') {
    return true;
  }
  if (left == '^' && (right != '~' && right != '^')) {
    return true;
  }
  if ((left == '*' || left == '/') && (right == '+' || right == '-')) {
    return true;
  }
  if ((left == '-' || left == '+') && right == '(') {
    return true;
  }

  return false;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s \"infix_expr\"\n", argv[0]);
    return 1;
  }

  char *input = argv[1];

  struct stack *s = stack_init();
  to_rpn(s, input);
  stack_cleanup(s);

  return 0;
}
