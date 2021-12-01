#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "../lock.h"


#define CYCLES 100000

int numberOfPhilosophers;
pthread_t * philosophers;
lock_t* forks;

void eating(int philosopherID) {
  printf("Philosopher n°%d is eating\n", philosopherID);

}

void * philosopher(void * arg) {

  int * id = (int * ) arg;
  int leftFork = * id;
  int rightFork = (leftFork + 1) % numberOfPhilosophers;

  for (int i = 0; i < CYCLES; i++) {
    if (leftFork < rightFork) {
      lock( & forks[leftFork]);
      printf("Philosopher n°%d took his left fork [%d]\n", * id, leftFork);
      lock( & forks[rightFork]);
      printf("Philosopher n°%d took his right fork [%d]\n", * id, rightFork);
    } else {
      lock( & forks[rightFork]);
      printf("Philosopher n°%d took his right fork [%d]\n", * id, rightFork);
      lock( & forks[leftFork]);
      printf("Philosopher n°%d took his left fork [%d]\n", * id, leftFork);
    }

    eating( * id);

    unlock( & forks[leftFork]);
    printf("Philosopher n°%d has freed his left fork [%d]\n", * id, leftFork);
    unlock( & forks[rightFork]);
    printf("Philosopher n°%d has freed his right fork [%d]\n", * id, rightFork);
  }

  return (NULL);
}

int main(int argc, char const * argv[]) {
  // HANDLING THE INPUT ARGUMENTS
  if (argc != 2) {
    printf("This program uses 1 argument.\n");
    exit(-1);
  }

  numberOfPhilosophers = atoi(argv[1]);

  //CREATING THE PHILOSOPHERS(threads) AND FORKS(mutexes) ARRAYS

  philosophers = (pthread_t * ) malloc(numberOfPhilosophers * sizeof(pthread_t));
  if (philosophers == NULL) {
    printf("Philosophers memory was not allocated\n");
    exit(0);
  } else {
    printf("Philosophers : %d \n", numberOfPhilosophers);
  }

  forks = (lock_t * ) malloc(numberOfPhilosophers * sizeof(lock_t));
  if (forks == NULL) {
    printf("Forks memory was not allocated\n");
    exit(0);
  } else {
    printf("Forks : %d\n", numberOfPhilosophers);
  }

  int err;



  //CREATING THE PHILOSOPHER THREADS

  for (int i = 0; i < numberOfPhilosophers; i++) {
    int * j = (int * ) malloc(sizeof(int));
    * j = i;
    err = pthread_create( & (philosophers[i]), NULL, philosopher, (void * ) j);
    if (err != 0) {
      printf("ERROR WHILE CREATING THREAD N°%d", i);
      exit(-1);
    } else {
      printf("THREAD N° %d CREATED\n", i);
    }
  }

  //JOINING THE PHILOSOPHER THREADS

  for (int i = 0; i < numberOfPhilosophers; i++) {
    err = pthread_join(philosophers[i], NULL);
    if (err != 0) {
      printf("ERROR WHILE JOINING THREAD N°%d", i);
      exit(-1);
    } else {
      printf("THREAD N° %d JOINED\n", i);
    }
  }

  //FREEING THE ARRAYS OF FORKS AND PHILOSOPHERS

  free(philosophers);
  free(forks);

  return 0;
}


