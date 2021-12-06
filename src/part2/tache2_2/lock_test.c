#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../lock.h"

#define NB_CRITICAL_SECTION_MAX 6400

int numberOfThreads;
pthread_t *threads;
lock_t mutex;
int err;

void *doingSomethingImportant(void *arg)
{
  int numberOfCriticalSections = NB_CRITICAL_SECTION_MAX / numberOfThreads;
  // int * id = (int * ) arg;
  for (int i = 0; i < numberOfCriticalSections; ++i)
  {
    lock(&mutex);
    // printf("Thread n° %d is working really hard\n", *id);
    while (rand() > RAND_MAX / 10000)
      ;
    unlock(&mutex);
  }
  return (NULL);
}

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    printf("This program uses 1 argument.\n");
    exit(-1);
  }
  numberOfThreads = atoi(argv[1]);

  threads = (pthread_t *)malloc(numberOfThreads * sizeof(pthread_t));
  if (threads == NULL)
  {
    printf("Threads array memory was not allocated\n");
    exit(0);
  }
  else
  {
    printf("Threads array created with %d threads \n", numberOfThreads);
  }

  for (int i = 0; i < numberOfThreads; i++)
  {
    int *j = (int *)malloc(sizeof(int));
    *j = i;
    err = pthread_create(&(threads[i]), NULL, &doingSomethingImportant, (void *)j);
    if (err != 0)
    {
      printf("ERROR WHILE CREATING THREAD N°%d", i);
      exit(-1);
    }
    else
    {
      // printf("********************THREAD N° %d CREATED********************\n", i);
    }
  }

  for (int i = 0; i < numberOfThreads; i++)
  {
    err = pthread_join(threads[i], NULL);
    if (err != 0)
    {
      printf("ERROR WHILE JOINING THREAD N°%d", i);
      exit(-1);
    }
    else
    {
      // printf("********************THREAD N° %d JOINED********************\n", i);
    }
  }

  return 0;
}