#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
  // You can ignore these options until/unless you implement the
  // bonus features.

  // Set to 1 if -y is specified, 0 otherwise.
  int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

typedef struct {
  char *name;
  int age;
} patient_t;

static int compare_patient_name(const void *a, const void *b) {
  return strcmp(((const patient_t *)a)->name, ((const patient_t *)b)->name);
}

static int compare_patient_age(const void *a, const void *b) {
  int res = ((const patient_t *)a)->age - ((const patient_t *)b)->age;
  if (res == 0) {
    return compare_patient_name(a,b);
  } else {
    return res;
  }
}

void free_patient(void *patient) {
  patient_t *p = (patient_t *)patient;
  if(p != NULL){
    if(p->name != NULL){
      free(p->name);
    }
    free(p);
  }
}

int main(int argc, char *argv[]) {
  prioq *queue;
  struct config cfg;

  if (parse_options(&cfg, argc, argv) != 0) {
    return EXIT_FAILURE;
  }

  if (cfg.year) {
    queue = prioq_init(&compare_patient_age);
  } else {
    queue = prioq_init(&compare_patient_name);
  }
  for (int iterations = 0;;) {
    while (1) {
      char *s = fgets(buf, BUF_SIZE, stdin);
      if (s == NULL) {
        fprintf(stderr, "Unexpected end of file. exiting\n");
        return EXIT_FAILURE;
      }
      if (strcmp(".\n", s) == 0) {
        patient_t *popped = (patient_t *)prioq_pop(queue);
        if(popped != NULL) {
          printf("%s\n", popped->name);
        }
        break;
      } else {
        char *name = strtok(buf," ");
        int age = atoi(strtok(NULL, " "));

        patient_t *p = malloc(sizeof(patient_t));
        if (p == NULL){
          prioq_cleanup(queue,free_patient);
          return EXIT_FAILURE;
        }

        p->name = malloc(strlen(name) + 1);
        if (p->name == NULL){
          free(p);
          prioq_cleanup(queue, free_patient);
          return EXIT_FAILURE;
        }
        strcpy(p->name, name);
        p->age = age;
        int res = prioq_insert(queue, p);
        if (res == 1){
          prioq_cleanup(queue,free_patient);
          return EXIT_FAILURE;
        }
      }
    }

    printf(".\n"); // End turn.

    if (++iterations == 10) {
      patient_t *popped = (patient_t *)prioq_pop(queue);
      while (popped != NULL) {
        printf("%s\n", popped->name);
        free_patient(popped);
        popped = (patient_t *)prioq_pop(queue);
      }
      break;
    }
  }
  prioq_cleanup(queue, free_patient);

  return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
  memset(cfg, 0, sizeof(struct config));
  int c;
  while ((c = getopt(argc, argv, "y")) != -1) {
    switch (c) {
    case 'y':
      cfg->year = 1;
      break;
    default:
      fprintf(stderr, "invalid option: -%c\n", optopt);
      return 1;
    }
  }
  return 0;
}
