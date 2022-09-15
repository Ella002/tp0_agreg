#include <stdio.h>

#ifndef MANIP_H
  #include "manip_buffer.h"
  #define MANIP_H
#endif

void compare_signature (buf_t* buf, unsigned int sign, char* name) {
  if (buf->tab[0] != sign)
    printf("Problem with %s. Signature expected: %u, result: %u.\n", name, sign, buf->tab[0]);
  else
    printf("Signature correct for %s.\n", name);
}

void compare_ptr (unsigned int res, unsigned int exp, char* name) {
  if (res != exp)
    printf("Problem with %s. Pointer expected: %u, result: %u.\n", name, exp, res);
  else
    printf("Pointer correct for %s.\n", name);
}

void compare_elt (buf_t* buf, unsigned int i, unsigned int exp, char* name) {

  if (buf->tab[i] != exp)
    printf("Problem with %s. buf->tab[%u] expected: %u, result: %u.\n",
	   name, i, exp, buf->tab[i]);
  else
    printf("buf->tab[%i] correct for %s.\n", i, name);
}
