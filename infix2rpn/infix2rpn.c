#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define LEFT_PARENTHESIS '('
#define RIGHT_PARENTHESIS ')'
#define MULTIPLICATION '*'
#define DIVISION '/'
#define ADDITION '+'
#define SUBSTRACTION '-'
#define NEGATION '~'
#define EXPONENTIATION '^'

bool is_valid(char *input);
bool has_precedence(char left, char right);
void print_rpn(struct stack *s, char *input);

bool is_valid(char *input) {
  int amount_left_parenthesis = 0;
  int amount_right_parenthesis = 0;
  for (size_t i = 0; i < strlen(input); i++) {
    if (input[i] == LEFT_PARENTHESIS) {
      amount_left_parenthesis++;
    } else if (input[i] == RIGHT_PARENTHESIS) {
      amount_right_parenthesis++;
    }
  }
  return amount_left_parenthesis == amount_right_parenthesis;
}

bool has_precedence(char left, char right) {
  if (left == NEGATION && right != NEGATION) {
    return true;
  }
  if (left == EXPONENTIATION &&
      (right != EXPONENTIATION && right != NEGATION)) {
    return true;
  }
  if ((left == MULTIPLICATION || left == DIVISION) &&
      (right == ADDITION || right == SUBSTRACTION)) {
    return true;
  }
  if ((left == SUBSTRACTION || left == ADDITION) && right == LEFT_PARENTHESIS) {
    return true;
  }

  return false;
}

void print_rpn(struct stack *s, char *input) {
  for (size_t i = 0; i < strlen(input); i++) {
    char c = input[i];
    // If it's a whitespace, just ignore it and skip a loop iteration.
    if (c == ' ') {
      continue;
    }

    if (isdigit(c)) {
      printf("%c", c);
      // If the next char is a digit, it means it's part of the previous char,
      // i.e. they form a double (or even triple, etc) digit. As such,
      // it can be simply printed and skipped in the for-loop iteration.
      while (isdigit(input[i + 1])) {
        printf("%c", input[++i]);
      }
      // Print a space to split the digits from the operators/other digits.
      printf(" ");
    } else if (c == LEFT_PARENTHESIS) {
      stack_push(s, c);
    } else if (c == RIGHT_PARENTHESIS) {
      while (stack_peek(s) != LEFT_PARENTHESIS) {
        printf("%c ", stack_pop(s));
      }
      // Pop the stack to get rid off the left parenthesis.
      stack_pop(s);
    } else {
      if (stack_empty(s)) {
        stack_push(s, c);
        continue;
      }
      // Also ensure that stack is not empty, i.e. if current operator has no
      // precedence over any operator in the stack it will get empty eventually.
      while (!stack_empty(s) && stack_peek(s) != LEFT_PARENTHESIS &&
             !has_precedence(c, stack_peek(s))) {
        printf("%c ", stack_pop(s));
      }
      stack_push(s, c);
    }
  }
  // At the end, completely empty out the stack.
  while (!stack_empty(s)) {
    printf("%c ", stack_pop(s));
  }
  // Separate from stderr and force a flush on the stdout stream.
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s \"infix_expr\"\n", argv[0]);
    return 1;
  }

  char *input = argv[1];
  if (!is_valid(input)) {
    return 1;
  }
  struct stack *s = stack_init();
  print_rpn(s, input);
  stack_cleanup(s);

  return 0;
}
