#include <stdlib.h>

#include "../semaphore.h"

int sem_init(sem_t *__sem, int __pshared, unsigned int __value)
{
  __sem->size = __value;
  __sem->lock = UNLOCKED;

  return 0;
}

int sem_destroy(sem_t *__sem)
{
  // Nothing to do. Defined just for compatibility
  return 0;
}

int sem_wait(sem_t *__sem)
{
  int sem_ok = 0;
  lock(&(__sem->lock));
  if (__sem->size) {
    __sem->size--;
    sem_ok = 1;
  }
  unlock(&(__sem->lock));

  if (!sem_ok) {
    // while(rand() > RAND_MAX/10000);
    sem_wait(__sem);
  }

  return 0;
}

int sem_post(sem_t *__sem)
{
  lock(&(__sem->lock));
  __sem->size++;
  unlock(&(__sem->lock));

  return 0;
}