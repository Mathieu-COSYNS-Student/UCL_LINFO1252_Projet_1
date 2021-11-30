#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "../../common/utils.h"
#include "../lock.h"
#include "../semaphore.h"

#define MAX_READINGS 2560
#define MAX_WRITINGS 640

lock_t readings_mutex, writings_mutex;
int readings_count = 0;
int writings_count = 0;


lock_t readcount_mutex, writecount_mutex;
int readcount = 0;
int writecount = 0;

sem_t z, rsem, wsem;

// int sh_var = 1;

bool has_readings()
{
  bool has_readings = false;

  lock(&readings_mutex);
  if (readings_count < MAX_READINGS) {
    readings_count++;
    has_readings = true;
  }
  unlock(&readings_mutex);

  return has_readings;
}

void before_read()
{
  sem_wait(&z);
  sem_wait(&rsem);
  lock(&readcount_mutex);
  if (!readcount)
    sem_wait(&wsem);
  readcount++;
  unlock(&readcount_mutex);
  sem_post(&rsem);
  sem_post(&z);
}

void read()
{
  while(rand() > RAND_MAX/10000);
  // printf("read number : %d, updated value : %d\n", readings_count, sh_var);
}

void after_read()
{
  lock(&readcount_mutex);
  readcount--;
  if (!readcount)
    sem_post(&wsem);
  unlock(&readcount_mutex);
}

void *reader(void *i)
{
  // int *nb = (int*)i;
  // printf("****R%d****\n", *nb);

  while (has_readings()) {
    before_read();
    read();
    after_read();
  }

  pthread_exit(NULL);
}

bool has_writings()
{
  bool has_writings = false;

  lock(&writings_mutex);
  if (writings_count < MAX_WRITINGS) {
    writings_count++;
    has_writings = true;
  }
  unlock(&writings_mutex);

  return has_writings;
}

void before_write()
{
  lock(&writecount_mutex);
  if (!writecount)
    sem_wait(&rsem);
  writecount++;
  unlock(&writecount_mutex);
  sem_wait(&wsem);
}

void write()
{
  while(rand() > RAND_MAX/10000);
  // printf("write number : %d\n", writings_count);
  // sh_var++;
}

void after_write()
{
  sem_post(&wsem);
  lock(&writecount_mutex);
  writecount--;
  if (!writecount)
    sem_post(&rsem);
  unlock(&writecount_mutex);
}

void *writer(void *i)
{
  // int *nb = (int*)i;
  // printf("****W%d****\n", *nb);

  while(has_writings()) {
    before_write();
    write();
    after_write();
  }

  pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s NUMBER_OF_READERS NUMBER_OF_WRITERS\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int number_of_readers = atoi(argv[1]);
  int number_of_writers = atoi(argv[2]);

  // printf("R=%d, W=%d\n", number_of_readers, number_of_writers);

  sem_init(&wsem, 0, 1);
  sem_init(&z, 0, 1);
  sem_init(&rsem, 0, 1);
  readings_mutex = UNLOCKED;
  writings_mutex = UNLOCKED;
  readcount_mutex = UNLOCKED;
  writecount_mutex = UNLOCKED;

  pthread_t* threads = (pthread_t*) safe_malloc((number_of_readers+number_of_writers)*sizeof(pthread_t));
  int* reader_writer_number = (int*) safe_malloc((number_of_readers+number_of_writers)*sizeof(int));

  int thread_counter = 0;

  for (int i = 0; i < number_of_readers; i++)
  {
    reader_writer_number[thread_counter] = i;
    pthread_create(threads+thread_counter, NULL, reader, (void *)(reader_writer_number+thread_counter));
    thread_counter++;
  }

  for (int i = 0; i < number_of_writers; i++)
  {
    reader_writer_number[thread_counter] = i;
    pthread_create(threads+thread_counter, NULL, writer, (void *)(reader_writer_number+thread_counter));
    thread_counter++;
  }

  for (int i = 0; i < thread_counter; i++)
  {
    pthread_join(threads[i], NULL);
  }

  free(threads);
  free(reader_writer_number);

  return 0;
}
