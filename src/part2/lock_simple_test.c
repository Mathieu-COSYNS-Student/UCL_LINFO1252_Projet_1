#include <stdlib.h>
#include <stdio.h>

#include "lock.h"

lock_t lock_var = UNLOCKED;

int main(void)
{
  for (size_t i = 0; i < 50; i++)
  {
    lock(&lock_var);
    unlock(&lock_var);
    printf("%ld\n", i);
  }
}