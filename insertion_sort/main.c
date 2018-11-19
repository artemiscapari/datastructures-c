#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

struct config {
  // You can ignore these options until/unless you implement the
  // bonus features.

  // Set to 1 if -u is specified, 0 otherwise.
  int remove_duplicates;

  // Set to 1 if -S is specified, 0 otherwise.
  int add_sum;

  // Set to N if -s N is specified, 0 otherwise.
  int select_multiple;

  // Set to N if -x N is specified, 0 otherwise.
  int remove_multiple;

  // Set to N if -h N is specified, 0 otherwise.
  int show_first;

  // Set to N if -t N is specified, 0 otherwise.
  int show_last;

  // Set to 1 if -3 is specified, 0 otherwise.
  int scribble;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

#define BUF_SIZE 1024
static char buf[BUF_SIZE];

int add_to_list(struct list *l, struct config *cfg, int num) {
  if (cfg->remove_multiple > 0) {
    if (num % cfg->remove_multiple == 0)
      return 0;
  }
  if (cfg->scribble && (num == 51 || num == 69 || num == 42)) {
    return list_add_back(l, 666);
  }
  return list_add_back(l, num);
}

void print_list(struct list *l, struct config *cfg) {
  if (cfg->show_first > 0) {
    int counter = 0;
    for (struct node *cur = list_head(l); cur; cur = list_next(cur)) {
      if (counter == cfg->show_first)
        break;
      printf("%d\n", list_node_data(cur));
      counter++;
    }
    return;
  }
  // Logic for normal and show_last printing is same, only beggining node is
  // different.
  struct node *curr = list_head(l);
  if (cfg->show_last > 0) {
    int counter = 0;
    // Since we know the list length, we can use it to stop exactly at the
    // length - N element.
    while (list_length(l) - counter > cfg->show_last) {
      curr = list_next(curr);
      counter++;
    }
  }
  for (; curr; curr = list_next(curr)) {
    printf("%d\n", list_node_data(curr));
  }
  if (cfg->add_sum > 0) {
    int sum = 0;
    for (struct node *cur = list_head(l); cur; cur = list_next(cur)) {
      sum += list_node_data(cur);
    }
    printf("%d\n", sum);
  }
}

int sort_list(struct list *l) {
  struct node *curr = list_next(list_head(l));
  while (curr != NULL) {
    int curr_data = list_node_data(curr);
    struct node *prev = list_prev(l, curr);
    bool must_swap = false;
    while (prev != NULL && list_node_data(prev) > curr_data) {
      prev = list_prev(l, prev);
      must_swap = true;
    }
    // Optimisation: if the node is already properly aligned, skip an iteration.
    if (must_swap) {
      // Temporarily obtain a pointer to the next one as the current will be
      // moved.
      struct node *tmp = list_next(curr);
      // Unlink the current node. It is going to be placed either in the
      // beginning (if lowest) or just after the first lower number.
      list_unlink_node(l, curr);
      if (prev == NULL) {
        int res = list_add(l, list_node_data(curr));
        list_free_node(curr);
        if (res == 1)
          return res;
      } else {
        int res = list_insert_after(l, curr, prev);
        if (res == 1)
          return res;
      }
      curr = tmp;
    } else {
      curr = list_next(curr);
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {

  struct config cfg;
  if (parse_options(&cfg, argc, argv) != 0)
    return 1;

  char *token;
  struct list *l = list_init();
  while (fgets(buf, BUF_SIZE, stdin)) {
    token = strtok(buf, " \n");
    while (token != NULL) {
      int res = add_to_list(l, &cfg, atoi(token));
      if (res == 1)
        return res;
      token = strtok(NULL, " \n");
    }
  }
  if (sort_list(l) == 1)
    return 1;

  print_list(l, &cfg);
  list_cleanup(l);
  return 0;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
  memset(cfg, 0, sizeof(struct config));
  int c;
  while ((c = getopt(argc, argv, "uSs:x:h:t:3")) != -1)
    switch (c) {
    case 'u':
      cfg->remove_duplicates = 1;
      break;
    case 'S':
      cfg->add_sum = 1;
      break;
    case '3':
      cfg->scribble = 1;
      break;
    case 's':
      cfg->select_multiple = atoi(optarg);
      break;
    case 'x':
      cfg->remove_multiple = atoi(optarg);
      break;
    case 'h':
      cfg->show_first = atoi(optarg);
      break;
    case 't':
      cfg->show_last = atoi(optarg);
      break;
    default:
      fprintf(stderr, "invalid option: -%c\n", optopt);
      return 1;
    }
  if (cfg->show_first != 0 && cfg->show_last != 0) {
    fprintf(stderr, "cannot specify both -h and -t\n");
    return 1;
  }
  return 0;
}
