#include <stdlib.h>
#include <stdio.h>

#include "lock.h"

int main(void)
{
  for (size_t i = 0; i < 50; i++)
  {
    lock();
    unlock();
    printf("%ld\n", i);
  }
}