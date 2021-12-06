#include <stdio.h>
#include <stdbool.h>

#include "../utils.h"

#define MAX_READINGS 2560
#define MAX_WRITINGS 640

mutex_t readings_mutex, writings_mutex;
int readings_count = 0;
int writings_count = 0;

mutex_t readcount_mutex, writecount_mutex;
int readcount = 0;
int writecount = 0;

sem_t z, rsem, wsem;

// int sh_var = 1;

bool has_readings()
{
  bool has_readings = false;

  safe_mutex_lock(&readings_mutex);
  if (readings_count < MAX_READINGS)
  {
    readings_count++;
    has_readings = true;
  }
  safe_mutex_unlock(&readings_mutex);

  return has_readings;
}

void before_read()
{
  safe_sem_wait(&z);
  safe_sem_wait(&rsem);
  safe_mutex_lock(&readcount_mutex);
  if (!readcount)
    safe_sem_wait(&wsem);
  readcount++;
  safe_mutex_unlock(&readcount_mutex);
  safe_sem_post(&rsem);
  safe_sem_post(&z);
}

void read()
{
  while (rand() > RAND_MAX / 10000)
    ;
  printf("read number : %d\n", readings_count);
}

void after_read()
{
  safe_mutex_lock(&readcount_mutex);
  readcount--;
  if (!readcount)
    safe_sem_post(&wsem);
  safe_mutex_unlock(&readcount_mutex);
}

void *reader(void *i)
{
  // int *nb = (int*)i;
  // printf("****R%d****\n", *nb);

  while (has_readings())
  {
    before_read();
    read();
    after_read();
  }

  pthread_exit(NULL);
}

bool has_writings()
{
  bool has_writings = false;

  safe_mutex_lock(&writings_mutex);
  if (writings_count < MAX_WRITINGS)
  {
    writings_count++;
    has_writings = true;
  }
  safe_mutex_unlock(&writings_mutex);

  return has_writings;
}

void before_write()
{
  safe_mutex_lock(&writecount_mutex);
  if (!writecount)
    safe_sem_wait(&rsem);
  writecount++;
  safe_mutex_unlock(&writecount_mutex);
  safe_sem_wait(&wsem);
}

void write()
{
  while (rand() > RAND_MAX / 10000)
    ;
  printf("write number : %d\n", writings_count);
  // sh_var++;
}

void after_write()
{
  safe_sem_post(&wsem);
  safe_mutex_lock(&writecount_mutex);
  writecount--;
  if (!writecount)
    safe_sem_post(&rsem);
  safe_mutex_unlock(&writecount_mutex);
}

void *writer(void *i)
{
  // int *nb = (int*)i;
  // printf("****W%d****\n", *nb);

  while (has_writings())
  {
    before_write();
    write();
    after_write();
  }

  pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s NUMBER_OF_READERS NUMBER_OF_WRITERS\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int number_of_readers = atoi(argv[1]);
  int number_of_writers = atoi(argv[2]);

  // printf("R=%d, W=%d\n", number_of_readers, number_of_writers);

  safe_sem_init(&wsem, 0, 1);
  safe_sem_init(&z, 0, 1);
  safe_sem_init(&rsem, 0, 1);
  safe_mutex_init(&readings_mutex, NULL);
  safe_mutex_init(&writings_mutex, NULL);
  safe_mutex_init(&readcount_mutex, NULL);
  safe_mutex_init(&writecount_mutex, NULL);

  pthread_t *threads = (pthread_t *)safe_malloc((number_of_readers + number_of_writers) * sizeof(pthread_t));
  int *reader_writer_number = (int *)safe_malloc((number_of_readers + number_of_writers) * sizeof(int));

  int thread_counter = 0;

  for (int i = 0; i < number_of_readers; i++)
  {
    reader_writer_number[thread_counter] = i;
    safe_pthread_create(threads + thread_counter, NULL, reader, (void *)(reader_writer_number + thread_counter));
    thread_counter++;
  }

  for (int i = 0; i < number_of_writers; i++)
  {
    reader_writer_number[thread_counter] = i;
    safe_pthread_create(threads + thread_counter, NULL, writer, (void *)(reader_writer_number + thread_counter));
    thread_counter++;
  }

  for (int i = 0; i < thread_counter; i++)
  {
    safe_pthread_join(threads[i], NULL);
  }

  free(threads);
  free(reader_writer_number);

  return 0;
}
