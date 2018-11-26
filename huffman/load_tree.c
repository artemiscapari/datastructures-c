#include "huffman.h"
#include "stack.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

tree_t load_tree(char *input) {
  struct stack *s = stack_init();
  for (size_t i = 0; i < strlen(input); i++) {
    char c = input[i];
    if (c != SPECIAL_CHAR && c != '\n') {
      // Push the char as a leaf onto the stack.
      stack_push(s, make_tree(c, 0, 0, 0));
      continue;
    } else if (c == SPECIAL_CHAR) {
      tree_t right = stack_pop(s);
      tree_t left = stack_pop(s);
      tree_t combined = make_tree(0, 0, left, right);
      stack_push(s, combined);
    }
  }
  tree_t root = stack_pop(s);
  stack_cleanup(s);
  return root;
}
