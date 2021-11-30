#include <stdlib.h>

#include "../lock.h"

void lock(lock_t* lock_ptr)
{
  asm (
      "movl    $1, %%eax;"
    "test_and_set:"
      "xchgl   %%eax, %0;"
      "testl   %%eax, %%eax;"
      "jnz     test_and_set;"
      :"=m"(*lock_ptr)  /* output operand */
      :"m"(*lock_ptr)   /* input operand */
      :"%eax" /* %eax is clobbered register */
  );
}

void unlock(lock_t* lock_ptr)
{
  asm (
      "movl    $0, %%eax;"
      "xchgl   %%eax, %1;"
      :"=m"(*lock_ptr)  /* output operand */
      :"m"(*lock_ptr)   /* input operand */
      :"%eax" /* %eax is clobbered register */
  );
}
