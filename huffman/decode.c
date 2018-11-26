#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *my_get_line(void) {
  char *line = 0;
  size_t n = 0;
  if (-1 == getline(&line, &n, stdin)) {
    perror("getline");
    free(line);
    exit(1);
  }
  return line;
}

void decode_tree(tree_t root, char *input) {
  tree_t current = root;
  for (size_t i = 0; i < strlen(input); i++) {
    char c = input[i];
    if (c == SPECIAL_CHAR) break;
    if (c == '0') current = current->left;
    if (c == '1') current = current->right;

    // If at a leaf, print the letter and return back to root.
    if (!current-> left && !current->right){
      putchar(current->letter);
      current = root;
    }
  }
}

int main(void) {
  char *tree_line = my_get_line();
  char *encoded_input = my_get_line();

  tree_t tree = load_tree(tree_line);

  if (tree == 0) {
    tree = fixed_tree();
  }
  decode_tree(tree, encoded_input);

  // Cleanup.
  free(tree_line);
  free(encoded_input);
  free_tree(tree);

  return 0;
}
