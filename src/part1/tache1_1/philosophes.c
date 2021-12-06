#include <stdio.h>
#include <stdbool.h>

#include "../utils.h"

#define CYCLES 100000

int numberOfPhilosophers;
pthread_t *philosophers;
mutex_t *forks;

void eating(int philosopherID)
{
  printf("Philosopher n°%d is eating\n", philosopherID);
}

void *philosopher(void *arg)
{

  int *id = (int *)arg;
  int leftFork = *id;
  int rightFork = (leftFork + 1) % numberOfPhilosophers;

  for (int i = 0; i < CYCLES; i++)
  {
    if (leftFork < rightFork)
    {
      safe_mutex_lock(&forks[leftFork]);
      printf("Philosopher n°%d took his left fork [%d]\n", *id, leftFork);
      safe_mutex_lock(&forks[rightFork]);
      printf("Philosopher n°%d took his right fork [%d]\n", *id, rightFork);
    }
    else
    {
      safe_mutex_lock(&forks[rightFork]);
      printf("Philosopher n°%d took his right fork [%d]\n", *id, rightFork);
      safe_mutex_lock(&forks[leftFork]);
      printf("Philosopher n°%d took his left fork [%d]\n", *id, leftFork);
    }

    eating(*id);

    safe_mutex_unlock(&forks[leftFork]);
    printf("Philosopher n°%d has freed his left fork [%d]\n", *id, leftFork);
    safe_mutex_unlock(&forks[rightFork]);
    printf("Philosopher n°%d has freed his right fork [%d]\n", *id, rightFork);
  }

  return (NULL);
}

int main(int argc, char const *argv[])
{
  // HANDLING THE INPUT ARGUMENTS
  if (argc != 2)
  {
    printf("This program uses 1 argument. Param : NUMBER OF PHILOSOPHERS(Threads)\n");
    exit(-1);
  }

  numberOfPhilosophers = atoi(argv[1]);

  //CREATING THE PHILOSOPHERS(threads) AND FORKS(mutexes) ARRAYS

  philosophers = (pthread_t *)malloc(numberOfPhilosophers * sizeof(pthread_t));
  forks = (mutex_t *)malloc(numberOfPhilosophers * sizeof(mutex_t));

  // INITIALIZING THE MUTEXES IN THE FORKS ARRAY

  for (int i = 0; i < numberOfPhilosophers; i++)
  {
    safe_mutex_init(&(forks[i]), NULL);
  }

  //CREATING THE PHILOSOPHER THREADS

  for (int i = 0; i < numberOfPhilosophers; i++)
  {
    int *j = (int *)safe_malloc(sizeof(int)); // TODO
    *j = i;
    safe_pthread_create(&(philosophers[i]), NULL, philosopher, (void *)j);
  }

  //JOINING THE PHILOSOPHER THREADS

  for (int i = 0; i < numberOfPhilosophers; i++)
  {
    safe_pthread_join(philosophers[i], NULL);
  }

  //DESTROYING THE FORK MUTEXES

  for (int i = 0; i < numberOfPhilosophers; i++)
  {
    safe_mutex_destroy(&(forks[i]));
  }

  //FREEING THE ARRAYS OF FORKS AND PHILOSOPHERS

  free(philosophers);
  free(forks);

  return 0;
}