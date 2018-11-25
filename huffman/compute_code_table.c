#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int get_int_length(int value){
  int length = 1;
  while(value){
    length++;
    value /=10;
  }
  return length;
}

void traverse_tree(tree_t node, int depth, int value, encoding_table_t *table) {
  if(!node) {
    return;
  }
  if(node->letter != 0){
    (*table)[node->letter].code_length = depth;
    sprintf((*table)[node->letter].code, "%0*d", depth, value);
  }
  depth +=1;
  traverse_tree(node->left, depth,(value << 1) + 0, table);
  traverse_tree(node->right, depth, (value << 1) + 1, table);

}

encoding_table_t* compute_code_table(tree_t root) {
    encoding_table_t *result = calloc(1, sizeof(encoding_table_t));
    if (result == 0) {
        perror("calloc");
        exit(1);
    }
    traverse_tree(root->left, 1, 0,result);
    traverse_tree(root->right, 1,1, result);
    printf("\n");
    return result;
}


void free_code_table(encoding_table_t* table) {

    // ... SOME CODE MISSING ...

    free(table);
}
