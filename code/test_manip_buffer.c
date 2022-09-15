#include <stdio.h>

#ifndef MANIP_H
  #include "manip_buffer.h"
  #define MANIP_H
#endif
#ifndef UTILS_H
  #include "utils.h"
  #define UTILS_H
#endif

/* On teste l'initialisation du buffer */
void test_init () {

  char* test = "test init";

  buf_t* buf = init(3);
  compare_elt(buf, 0, 0, test);
  compare_ptr(buf->ptr, 0, test);
  if (buf->taille != 3)
    printf("Problem with %s. Size expected: %u, result: %u.\n", test, 3, buf->taille);
  else
    printf("Size correct for %s.\n", test);

  delete(buf);
}

/* On teste le calcul de signature */
void test_sign () {

  buf_t* buf = init(4);

  char* test = "test sign [0]";
  // signature [0] = 0
  sign(buf);
  compare_signature(buf, 0, test);
  compare_ptr(buf->ptr, 0, test);

  buf->tab[1] = 4;
  buf->ptr = 1;

  test = "test sign [0; 4]";
  // signature [0; 4] = 0 ^ 4 = 4
  sign(buf);
  compare_signature(buf, 4, test);
  compare_ptr(buf->ptr, 0, test);

  buf->tab[1] = 5;
  buf->tab[2] = 3;
  buf->ptr = 2;

  test = "test sign [4; 5; 3]";
  // signature [4; 5; 3] = 0b100 ^ 0b101 ^ 0b011 = 0b010 = 2
  sign(buf);
  compare_signature(buf, 2, test);
  compare_ptr(buf->ptr, 0, test);

  delete(buf);
}

/* On teste l'ajout d'un element */
void test_add_x () {

  buf_t* buf = init(4);

  char* test = "test add 5 to [0]";
  add_x(buf, 5);
  compare_elt(buf, 0, 0, test);
  compare_elt(buf, 1, 5, test);
  compare_ptr(buf->ptr, 1, test);

  test = "test add 4 & 3 to [0; 5]";
  add_x(buf, 4);
  add_x(buf, 3);
  compare_elt(buf, 2, 4, test);
  compare_elt(buf, 3, 3, test);
  compare_ptr(buf->ptr, 3, test);

  test = "test add 42 to [0; 5; 4; 3]";
  // le tampon est plein, il faut signer puis ajouter 42
  add_x(buf, 42);
  compare_signature(buf, 2, test);
  compare_elt(buf, 1, 42, test);
  compare_ptr(buf->ptr, 1, test);

  delete(buf);
}

int main (int argc, char* argv[]) {

  test_init();
  test_sign();
  test_add_x();
  return 0;

}
