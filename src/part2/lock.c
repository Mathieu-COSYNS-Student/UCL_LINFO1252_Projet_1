#include <stdlib.h>
#include <stdio.h>

#include "lock.h"

int lock_var = 0;

void lock()
{
  asm (
      "movl    $1, %%eax;"
    "enter:"
      "xchgl   %%eax, %0;"
      "testl   %%eax, %%eax;"
      "jnz     enter;"
      :"=m"(lock_var)  /* output operand */
      :"m"(lock_var)   /* input operand */
      :"%eax" /* %eax is clobbered register */
  );
}

void unlock()
{
  asm (
      "movl    $0, %%eax;"
      "xchgl   %%eax, %1;"
      :"=m"(lock_var)  /* output operand */
      :"m"(lock_var)   /* input operand */
      :"%eax" /* %eax is clobbered register */
  );
}
