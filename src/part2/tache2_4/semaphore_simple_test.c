#include <stdio.h>

#include "semaphore.h"

int main(int argc, char const *argv[])
{
  sem_t sem;

  sem_init(&sem, 0, 1);

  sem_wait(&sem);
  sem_post(&sem);

  sem_destroy(&sem);

  printf("END\n");
  return 0;
}
