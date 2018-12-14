#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
  struct node *root;
  int turbo;
};

struct node {
  int data;
  struct node *lhs;
  struct node *rhs;
  struct node *parent;
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

int is_leaf(node *node) {
  return node->lhs == NULL && node->rhs == NULL ? 1 : 0;
}

node *get_node_add(node *current, int data) {
  if (current->data > data) {
    return !current->lhs ? current : get_node_add(current->lhs, data);
  } else {
    return !current->rhs ? current : get_node_add(current->rhs, data);
  }
}

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data) {
  node *n = malloc(sizeof(node));
  if (n == NULL) {
    return NULL;
  }
  n->lhs = NULL;
  n->rhs = NULL;
  n->parent = NULL;
  n->data = data;
  return n;
}

node *traverse_node_r(node *root, int data) {
  if (root->data == data) {
    return root;
  } else if (root->lhs != NULL && root->data > data) {
    return traverse_node_r(root->lhs, data);
  } else if (root->rhs != NULL && root->data < data) {
    return traverse_node_r(root->rhs, data);
  } else {
    return NULL;
  }
}

void print_tree_r(node *root) {
  if (root == NULL) {
    return;
  }

  print_tree_r(root->lhs);
  printf("%d\n", root->data);
  print_tree_r(root->rhs);
}

static int print_tree_dot_r(node *root, FILE *dotf) {
  int left_id, right_id, my_id = global_node_counter++;

  if (root == NULL) {
    fprintf(dotf, "    %d [shape=point];\n", my_id);
    return my_id;
  }

  fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

  left_id = print_tree_dot_r(root->lhs, dotf);
  fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

  right_id = print_tree_dot_r(root->rhs, dotf);
  fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

  return my_id;
}

void tree_dot(struct tree *tree, char *filename) {
  node *root = tree->root;
  global_node_counter = 0;
  FILE *dotf = fopen(filename, "w");
  if (!dotf) {
    printf("error opening file: %s\n", filename);
    exit(1);
  }
  fprintf(dotf, "digraph {\n");
  if (root) {
    print_tree_dot_r(root, dotf);
  }
  fprintf(dotf, "}\n");
  fclose(dotf);
}

int tree_check(struct tree *tree) {
  return tree != NULL ? 0 : 1;
}

struct tree *tree_init(int turbo) {
  struct tree *t = malloc(sizeof(struct tree));
  if (!t) {
    return NULL;
  }
  t->turbo = turbo;
  t->root = NULL;
  return t;
}

int tree_insert(struct tree *tree, int data) {
  if (tree->root == NULL) {
    node *n = make_node(data);
    if (n == NULL) {
      return -1;
    }
    tree->root = n;
    return 0;
  } else {
    if (tree_find(tree, data) == 1) {
      return 1;
    }
    node *current = get_node_add(tree->root, data);
    if (!current) {
      return -1;
    }
    if (current->data > data) {
      node *new_node = make_node(data);
      current->lhs = new_node;
      new_node->parent = current;
      return current->lhs == NULL ? -1 : 0;
    } else {
      node *new_node = make_node(data);
      current->rhs = new_node;
      new_node->parent = current;
      return current->rhs == NULL ? -1 : 0;
    }
  }
}

int tree_find(struct tree *tree, int data) {
  if (tree->root == NULL) {
    return 0;
  }

  return traverse_node_r(tree->root, data) == NULL ? 0 : 1;
}

int tree_remove(struct tree *tree, int data) {
  if (tree->root == NULL) {
    return 1;
  }

  node *n = traverse_node_r(tree->root, data);
  if (n == NULL) {
    return 1;
  }

  /* If it is a leaf then only thing that has to be done is unlink it from
   * the parent. */
  if (is_leaf(n)) {
    if (n->parent == NULL){
      // If no leaf or parent then this is the root and a single node.
      tree->root = NULL;
    } else {
      if(n->parent->lhs != NULL && n->parent->lhs->data == data) {
        n->parent->lhs = NULL;
      } else if (n->parent->rhs != NULL && n->parent->rhs->data == data) {
        n->parent->rhs = NULL;
      }
    }
    free(n);
  } else if (n->lhs != NULL && n->rhs != NULL) {
    /* If the node is not a leaf and it has two children then find the
     * second biggest next to N and swap them. Then delete the swapped leaf. */
    node *next = n->rhs;
    while (next->lhs != NULL) {
      next = next->lhs;
    }
    if(n->parent == NULL){
      tree->root = next;
      next->lhs = n->lhs;
      free(n);
    } else {
      if(is_leaf(next)) {
        n->data = next->data;
      } else {
        next->parent->lhs = next->rhs;
        n->data = next->data;
      }
      free(next);
    }
  } else {
    /* If there is only a single child, replace the parent's respective side
     * with it */
    node *parent = n->parent;
    if(parent == NULL) {
        tree->root = n->rhs != NULL ? n->rhs : n->lhs;
    } else {
      if(parent->rhs != NULL && parent->rhs->data==n->data){
        parent->rhs = n->rhs != NULL ? n->rhs : n->lhs;
      } else {
        parent->lhs = n->rhs != NULL ? n->rhs : n->lhs;
      }
    }
    free(n);
  }
  return 0;
}

void tree_print(struct tree *tree) {
  print_tree_r(tree->root);
}

void cleanup_tree_r(node *current) {
  if (current == NULL) {
    return;
  }
  if (is_leaf(current)) {
    free(current);
    return;
  }
  cleanup_tree_r(current->lhs);
  cleanup_tree_r(current->rhs);

  free(current);
}

void tree_cleanup(struct tree *tree) {
  if (tree != NULL) {
    if (tree->root != NULL) {
      cleanup_tree_r(tree->root);
    }
    free(tree);
  }
}
