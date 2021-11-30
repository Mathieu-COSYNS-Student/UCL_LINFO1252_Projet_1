#include "lock.h"

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct {
  int size;
  lock_t lock;
} sem_t;

int sem_init(sem_t *__sem, int __pshared, unsigned int __value);

int sem_destroy(sem_t *__sem);

int sem_wait(sem_t *__sem);

int sem_post(sem_t *__sem);

#endif // !SEMAPHORE_H