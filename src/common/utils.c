#include <stdlib.h>
#include <stdio.h>

void* safe_malloc(size_t size) {
  void* allocated_men = malloc(size);

  if (!size && !allocated_men)
    return allocated_men;

  if(!allocated_men) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  return allocated_men;
}