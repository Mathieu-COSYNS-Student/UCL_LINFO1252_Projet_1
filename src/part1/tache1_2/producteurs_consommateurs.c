#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include <semaphore.h>

#define BUFFER_SIZE 8
#define ROUNDS 1024

int producer_round_number = 0;
pthread_mutex_t producer_round_mutex;

int consumer_round_number = 0;
pthread_mutex_t consumer_round_mutex;

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

int numberOfProducers;
int numberOfConsumers;

pthread_t * producers;
pthread_t * consumers;

int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

bool can_produce() {
  bool can_produce = false;

  pthread_mutex_lock( & producer_round_mutex);
  if (producer_round_number < ROUNDS) {
    producer_round_number++;
    can_produce = true;
  }
  pthread_mutex_unlock( & producer_round_mutex);

  return can_produce;
}

bool can_consume() {
  bool can_consume = false;

  pthread_mutex_lock( & consumer_round_mutex);
  if (consumer_round_number < ROUNDS) {
    consumer_round_number++;
    can_consume = true;
  }
  pthread_mutex_unlock( & consumer_round_mutex);

  return can_consume;
}

void * producer(void * arg) {
  int * num = (int * ) arg;

  while (can_produce()) {
    sem_wait( & empty); // WAITING FOR AN EMPTY SLOT
    pthread_mutex_lock( & mutex);
    // section critique
    printf("Producer %d: Producing in slot n° %d\n", * num, in );
    buffer[ in ] = 1;
    in = ( in +1) % BUFFER_SIZE;
    pthread_mutex_unlock( & mutex);
    sem_post( & full); // INDICATING THAT A SPOT HAS BEEN FILLED
    while (rand() > RAND_MAX / 10000); 
  }
  return (NULL);
}

void * consumer(void * arg) {
  int * num = (int * ) arg;

  while (can_consume()) {
    sem_wait( & full); // WAITING FOR AN OCCUPIED SPOT
    pthread_mutex_lock( & mutex);
    // section critique
    printf("Consumer %d: Consuming from slot n° %d\n", * num, out);
    buffer[out] = 0;
    out = (out + 1) % BUFFER_SIZE;
    pthread_mutex_unlock( & mutex);
    sem_post( & empty); // INDICATING THAT A SPOT HAS BEEN EMPTIED
    while (rand() > RAND_MAX / 10000);
  }

  return (NULL);
}

int main(int argc, char
  const * argv[]) {
  // HANDLING THE INPUT ARGUMENTS
  if (argc != 3) {
    printf("This program uses 2 arguments. Params : NUMBER_OF_PRODUCERS NUMBER_OF_CONSUMERS\n");
    exit(-1);
  }
  numberOfProducers = atoi(argv[1]);
  numberOfConsumers = atoi(argv[2]);

  //CREATING THE ARRAY OF PRODUCERS AND CONSUMMERS
  producers = (pthread_t * ) malloc(numberOfProducers * sizeof(pthread_t));
  if (producers == NULL) {
    printf("Producers memory was not allocated\n");
    exit(0);
  } else {
    printf("Producers : %d \n", numberOfProducers);
  }
  consumers = (pthread_t * ) malloc(numberOfConsumers * sizeof(pthread_t));
  if (consumers == NULL) {
    printf("Consumers memory was not allocated\n");
    exit(0);
  } else {
    printf("Consumers : %d \n", numberOfConsumers);
  }

  // INITIALIZING THE MUTEX, THE EMPTY SEMAPHORE AND THE FULL SEMAPHORE
  int err;

  err = pthread_mutex_init( & mutex, NULL);
  if (err != 0) {
    printf("ERROR WHILE INITIALIZING THE MUTEX");
    exit(-1);
  } else {
    printf("MUTEX CREATED\n");
  }

  err = pthread_mutex_init( & producer_round_mutex, NULL);
  if (err != 0) {
    printf("ERROR WHILE INITIALIZING THE MUTEX");
    exit(-1);
  } else {
    printf("MUTEX CREATED\n");
  }

  err = pthread_mutex_init( & consumer_round_mutex, NULL);
  if (err != 0) {
    printf("ERROR WHILE INITIALIZING THE MUTEX");
    exit(-1);
  } else {
    printf("MUTEX CREATED\n");
  }

  err = sem_init( & empty, 0, BUFFER_SIZE);
  if (err != 0) {
    printf("ERROR WHILE INITIALIZING THE EMPTY SEMAPHORE");
    exit(-1);
  } else {
    printf("EMPTY SEMAPHORE CREATED\n");
  }

  err = sem_init( & full, 0, 0);
  if (err != 0) {
    printf("ERROR WHILE INITIALIZING THE FULL SEMAPHORE");
    exit(-1);
  } else {
    printf("FULL SEMAPHORE CREATED\n");
  }

  //CREATING THE PRODUCER AND CONSUMER THREADS

  for (int i = 0; i < numberOfProducers; i++) {
    int * j = (int * ) malloc(sizeof(int));
    * j = i;
    err = pthread_create( & (producers[i]), NULL, & producer, (void * ) j);
    if (err != 0) {
      printf("ERROR WHILE CREATING PRODUCER THREAD N°%d", i);
      exit(-1);
    } else {
      printf("********************PRODUCER N° %d CREATED********************\n", i);
    }
  }

  for (int i = 0; i < numberOfConsumers; i++) {
    int * j = (int * ) malloc(sizeof(int));
    * j = i;
    err = pthread_create( & (consumers[i]), NULL, & consumer, (void * ) j);
    if (err != 0) {
      printf("ERROR WHILE CREATING CONSUMER THREAD N°%d", i);
      exit(-1);
    } else {
      printf("********************CONSUMER N° %d CREATED********************\n", i);
    }
  }

  //JOINING THE PRODUCER AND CONSUMMER THREADS

  for (int i = 0; i < numberOfProducers; i++) {
    err = pthread_join(producers[i], NULL);
    if (err != 0) {
      printf("ERROR WHILE JOINING PRODUCER THREAD N°%d", i);
      exit(-1);
    } else {

      printf("********************PRODUCER N° %d JOINED********************\n", i);
    }
  }

  for (int i = 0; i < numberOfConsumers; i++) {
    err = pthread_join(consumers[i], NULL);
    if (err != 0) {
      printf("ERROR WHILE JOINING CONSUMER THREAD N°%d", i);
    } else {
      printf("********************CONSUMER N° %d JOINED********************\n", i);
    }
  }

  //DESTROYING THE MUTEX

  err = pthread_mutex_destroy( & (mutex));
  if (err != 0) {
    printf("ERROR WHILE DESTROYING THE MUTEX");
  }

  //FREEING THE ARRAYS OF THREADS
  free(producers);
  free(consumers);

  return 0;
}