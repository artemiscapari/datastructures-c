#include "huffman.h"
#include <stdio.h>

void print_tree(tree_t root) {
  if (!root) return;

  print_tree(root->left);
  print_tree(root->right);
  putchar(root->letter != 0 ? root->letter : '~');
}
