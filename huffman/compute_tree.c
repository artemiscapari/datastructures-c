#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

tree_t compute_tree(frequency_table_t *freqs) {
  if(freqs == NULL) exit(1);
  return 0;
}

void free_tree(tree_t tree) {
  if (!tree) {
    return;
  }
  free_tree(tree->left);
  free_tree(tree->right);
  free(tree);
}
