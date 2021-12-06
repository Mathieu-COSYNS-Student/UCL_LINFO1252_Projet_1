#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef UTILS_H
#define UTILS_H

typedef pthread_mutex_t mutex_t;

void *safe_malloc(size_t size);

int safe_sem_init(sem_t *__sem, int __pshared, unsigned int __value);

int safe_sem_destroy(sem_t *__sem);

int safe_sem_wait(sem_t *__sem);

int safe_sem_post(sem_t *__sem);

int safe_mutex_init(mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

int safe_mutex_lock(mutex_t *mutex);

int safe_mutex_unlock(mutex_t *mutex);

int safe_mutex_destroy(mutex_t *mutex);

int safe_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

int safe_pthread_join(pthread_t thread, void **retval);

#endif // !UTILS_H