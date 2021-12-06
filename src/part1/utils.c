#include <stdlib.h>
#include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>

#include "utils.h"

void *safe_malloc(size_t size)
{
  void *allocated_men = malloc(size);

  if (!size && !allocated_men)
    return allocated_men;

  if (!allocated_men)
  {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  return allocated_men;
}

void check_sem(int return_val, const char *message)
{
  if (return_val == -1)
  {
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
  }
}

int safe_sem_init(sem_t *__sem, int __pshared, unsigned int __value)
{
  int return_val = sem_init(__sem, __pshared, __value);
  check_sem(return_val, "sem_init failed\n");
  return return_val;
}

int safe_sem_destroy(sem_t *__sem)
{
  int return_val = sem_destroy(__sem);
  check_sem(return_val, "sem_destroy failed\n");
  return return_val;
}

int safe_sem_wait(sem_t *__sem)
{
  int return_val = sem_wait(__sem);
  check_sem(return_val, "sem_wait failed\n");
  return return_val;
}

int safe_sem_post(sem_t *__sem)
{
  int return_val = sem_post(__sem);
  check_sem(return_val, "sem_post failed\n");
  return return_val;
}

void check_pthread(int return_val, const char *message)
{
  if (return_val)
  {
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
  }
}

int safe_mutex_init(mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
  int return_val = pthread_mutex_init(mutex, mutexattr);
  check_pthread(return_val, "pthread_mutex_init failed\n");
  return return_val;
}

int safe_mutex_lock(mutex_t *mutex)
{
  int return_val = pthread_mutex_lock(mutex);
  check_pthread(return_val, "pthread_mutex_lock failed\n");
  return return_val;
}

int safe_mutex_unlock(mutex_t *mutex)
{
  int return_val = pthread_mutex_unlock(mutex);
  check_pthread(return_val, "pthread_mutex_unlock failed\n");
  return return_val;
}

int safe_mutex_destroy(mutex_t *mutex)
{
  int return_val = pthread_mutex_destroy(mutex);
  check_pthread(return_val, "pthread_mutex_destroy failed\n");
  return return_val;
}

int safe_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
  int return_val = pthread_create(thread, attr, start_routine, arg);
  check_pthread(return_val, "pthread_create failed\n");
  return return_val;
}

int safe_pthread_join(pthread_t thread, void **retval)
{
  int return_val = pthread_join(thread, retval);
  check_pthread(return_val, "pthread_join failed\n");
  return return_val;
}