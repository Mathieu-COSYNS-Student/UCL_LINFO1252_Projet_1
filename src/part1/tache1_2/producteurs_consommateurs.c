#include <stdio.h>
#include <stdbool.h>

#include "../utils.h"

#define BUFFER_SIZE 8
#define ROUNDS 1024

int producer_round_number = 0;
mutex_t producer_round_mutex;

int consumer_round_number = 0;
mutex_t consumer_round_mutex;

mutex_t mutex;

sem_t empty;
sem_t full;

int numberOfProducers;
int numberOfConsumers;

pthread_t *producers;
pthread_t *consumers;

int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

bool can_produce()
{
  bool can_produce = false;

  safe_mutex_lock(&producer_round_mutex);
  if (producer_round_number < ROUNDS)
  {
    producer_round_number++;
    can_produce = true;
  }
  safe_mutex_unlock(&producer_round_mutex);

  return can_produce;
}

bool can_consume()
{
  bool can_consume = false;

  safe_mutex_lock(&consumer_round_mutex);
  if (consumer_round_number < ROUNDS)
  {
    consumer_round_number++;
    can_consume = true;
  }
  safe_mutex_unlock(&consumer_round_mutex);

  return can_consume;
}

void *producer(void *arg)
{
  int *num = (int *)arg;

  while (can_produce())
  {
    safe_sem_wait(&empty); // WAITING FOR AN EMPTY SLOT
    safe_mutex_lock(&mutex);
    // section critique
    printf("Producer %d: Producing in slot n° %d\n", *num, in);
    buffer[in] = 1;
    in = (in + 1) % BUFFER_SIZE;
    safe_mutex_unlock(&mutex);
    safe_sem_post(&full); // INDICATING THAT A SPOT HAS BEEN FILLED
    while (rand() > RAND_MAX / 10000)
      ;
  }
  return (NULL);
}

void *consumer(void *arg)
{
  int *num = (int *)arg;

  while (can_consume())
  {
    safe_sem_wait(&full); // WAITING FOR AN OCCUPIED SPOT
    safe_mutex_lock(&mutex);
    // section critique
    printf("Consumer %d: Consuming from slot n° %d\n", *num, out);
    buffer[out] = 0;
    out = (out + 1) % BUFFER_SIZE;
    safe_mutex_unlock(&mutex);
    safe_sem_post(&empty); // INDICATING THAT A SPOT HAS BEEN EMPTIED
    while (rand() > RAND_MAX / 10000)
      ;
  }

  return (NULL);
}

int main(int argc, char const *argv[])
{
  // HANDLING THE INPUT ARGUMENTS
  if (argc != 3)
  {
    printf("This program uses 2 arguments. Params : NUMBER_OF_PRODUCERS NUMBER_OF_CONSUMERS\n");
    exit(-1);
  }
  numberOfProducers = atoi(argv[1]);
  numberOfConsumers = atoi(argv[2]);

  //CREATING THE ARRAY OF PRODUCERS AND CONSUMMERS
  producers = (pthread_t *)safe_malloc(numberOfProducers * sizeof(pthread_t));

  consumers = (pthread_t *)safe_malloc(numberOfConsumers * sizeof(pthread_t));

  // INITIALIZING THE MUTEX, THE EMPTY SEMAPHORE AND THE FULL SEMAPHORE
  safe_mutex_init(&mutex, NULL);
  safe_mutex_init(&producer_round_mutex, NULL);
  safe_mutex_init(&consumer_round_mutex, NULL);
  safe_sem_init(&empty, 0, BUFFER_SIZE);
  safe_sem_init(&full, 0, 0);

  //CREATING THE PRODUCER AND CONSUMER THREADS

  for (int i = 0; i < numberOfProducers; i++)
  {
    int *j = (int *)safe_malloc(sizeof(int)); // TODO
    *j = i;
    safe_pthread_create(&(producers[i]), NULL, &producer, (void *)j);
    printf("********************PRODUCER N° %d CREATED********************\n", i);
  }

  for (int i = 0; i < numberOfConsumers; i++)
  {
    int *j = (int *)safe_malloc(sizeof(int)); // TODO
    *j = i;
    safe_pthread_create(&(consumers[i]), NULL, &consumer, (void *)j);
    printf("********************CONSUMER N° %d CREATED********************\n", i);
  }

  //JOINING THE PRODUCER AND CONSUMMER THREADS

  for (int i = 0; i < numberOfProducers; i++)
  {
    safe_pthread_join(producers[i], NULL);
    printf("********************PRODUCER N° %d JOINED********************\n", i);
  }

  for (int i = 0; i < numberOfConsumers; i++)
  {
    safe_pthread_join(consumers[i], NULL);
    printf("********************CONSUMER N° %d JOINED********************\n", i);
  }

  //DESTROYING THE MUTEX

  safe_mutex_destroy(&(mutex));

  //FREEING THE ARRAYS OF THREADS
  free(producers);
  free(consumers);

  return 0;
}