#include "huffman.h"
#include <stdio.h>
#include <string.h>

int print_code(code c) {
  if (c.code != NULL) {
    printf("%s", c.code);
    return (int)strlen(c.code);
  }
  return 0;
}
