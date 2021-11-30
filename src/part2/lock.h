#ifndef LOCK_H
#define LOCK_H

typedef enum { UNLOCKED = 0, LOCKED = 1 } lock_t;

void lock(lock_t* lock_var);
void unlock(lock_t* lock_var);

#endif // !LOCK_H