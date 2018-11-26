#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

void traverse_tree(tree_t node, int depth, int value, encoding_table_t *table) {
  if (!node) {
    return;
  }
  if (node->letter != 0) {
    (*table)[(int)node->letter].code_length = depth;
    (*table)[(int)node->letter].code = malloc((unsigned int)depth + 1);
    if ((*table)[(int)node->letter].code == NULL) {
      exit(1); // malloc failed
    }
    sprintf((*table)[(int)node->letter].code, "%0*d", depth, value);
  }
  depth++;
  traverse_tree(node->left, depth, (value << 1) + 0, table);
  traverse_tree(node->right, depth, (value << 1) + 1, table);
}

encoding_table_t *compute_code_table(tree_t root) {
  encoding_table_t *result = calloc(1, sizeof(encoding_table_t));
  if (result == 0) {
    perror("calloc");
    exit(1);
  }
  traverse_tree(root->left, 1, 0, result);
  traverse_tree(root->right, 1, 1, result);
  return result;
}

void free_code_table(encoding_table_t *table) {
  for (int i = 0; i < 256; i++) {
    if ((*table)[i].code != NULL) {
      free((*table)[i].code);
    }
  }
  free(table);
}
