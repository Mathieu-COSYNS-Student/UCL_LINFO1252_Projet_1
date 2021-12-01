#include <stdio.h>
#include <pthread.h>

#include "../../common/utils.h"
#include "semaphore.h"

sem_t* sem;

void* test()
{
  for (size_t i = 0; i < 1000000; i++)
  {
    sem_wait(sem);
    sem_post(sem);
  }

  pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
  sem = (sem_t*) safe_malloc(sizeof(sem_t));

  sem_init(sem, 0, 1);

  int nb = 2;

  pthread_t* threads = (pthread_t*) safe_malloc(nb*sizeof(pthread_t));

  for (size_t i = 0; i < nb; i++)
  {
    pthread_create(threads+i, NULL, test, NULL);
  }
  
  for (size_t i = 0; i < nb; i++)
  {
    pthread_join(threads[i], NULL);
  }
  

  free(threads);

  sem_destroy(sem);
  free(sem);

  printf("END\n");
  return 0;
}
