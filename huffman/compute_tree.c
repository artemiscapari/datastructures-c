#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

tree_t compute_tree(frequency_table_t *freqs) {
  tree_t forest[256] = { NULL };
  for (int i = 0; i < 256; i++) {
    if((*freqs)[i] == 0) continue;

    forest[i] = (*freqs)[i];
  }

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

void heapify(frequency_table_t *freqs, int i, int n){
  
}
