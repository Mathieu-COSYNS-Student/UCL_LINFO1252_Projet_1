#include <stdlib.h>
#include <stdio.h>

#include "../lock.h"

void lock(lock_t *lock_ptr)
{
  asm(
      "movl    $1, %%eax;"
      "movl    $0, %%ebx;"
      "jmp     test_and_set;"
      "test_and_test_and_set:"
      "cmp     %0, %%ebx;"
      "jne     test_and_test_and_set;"
      "test_and_set:"
      "xchgl   %%eax, %0;"             // set eax to zero if has locked
      "testl   %%eax, %%eax;"          // set ZF to 1 if eax is 0
      "jnz     test_and_test_and_set;" // jump if ZF not set
      : "=m"(*lock_ptr)                /* output operand */
      : "m"(*lock_ptr)                 /* input operand */
      : "%eax", "%ebx"                 /* %eax and %ebx are clobbered register */
  );
}

void unlock(lock_t *lock_ptr)
{
  asm(
      "movl    $0, %%eax;"
      "xchgl   %%eax, %1;"
      : "=m"(*lock_ptr) /* output operand */
      : "m"(*lock_ptr)  /* input operand */
      : "%eax"          /* %eax is clobbered register */
  );
}
