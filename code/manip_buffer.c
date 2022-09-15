#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef MANIP_H
  #include "manip_buffer.h"
  #define MANIP_H
#endif

#define SEC 1000000

pthread_mutex_t lock;

buf_t* init (int T, bool use_threads) {
  // on alloue la structure de buffer
  buf_t* buf = malloc(sizeof(buf_t));
  // on initialise les attributs en allouant le tableau
  buf->tab = malloc(T * sizeof(unsigned int));
  buf->ptr = 0;
  buf->taille = T;

  if (use_threads) {
    // on initialise le verrou mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
      printf("mutex init: error\n");
      exit(1);
    }
  }

  return buf;
}

void delete(buf_t* buf) {
  free(buf->tab);
  free(buf);
}

void print(buf_t* buf) {
  int cpt;
  printf("|");
  for (cpt = 0; cpt <= buf->ptr; cpt++) {
    printf(" %i |", buf->tab[cpt]);
  }
  for (; cpt < buf->taille; cpt++) {
    printf(" |");
  }
  printf("\n");
}

void sign (buf_t* buf) {

  printf("Signing...\n");

  unsigned int cpt;
  // pour chaque case du tableau
  for (cpt = 1; cpt <= buf->ptr; cpt++)
    // on calcule la signature en faisant le XOR avec la signature actuelle
    buf->tab[0] ^= buf->tab[cpt];
  // on remet le pointeur sur la premiere case
  buf->ptr = 0;

  print(buf);
}

void add_x (buf_t* buf, unsigned int x) {

  printf("Adding %i...\n", x);

  if (buf->ptr + 1 >= buf->taille) {
    // s'il n'y a plus de place, on commence par signer
    printf("Buffer full. ");
    sign(buf);
  }

  // on ajoute à la fin
  buf->tab[buf->ptr+1] = x;
  buf->ptr = buf->ptr + 1;

  print(buf);
}

void add_x_threads (void* void_arg) {

  add_x_params_t* args = (add_x_params_t*) void_arg;

  printf("Adding %i in a thread...\n", args->x);

  // simulation du calcul
  usleep(3 * SEC);

  // on utilise le mutex: un seul thread (executant cette fonction)
  // peut modifier le buffer en appelant add
  pthread_mutex_lock(&lock);
  add_x(args->buf, args->x);
  pthread_mutex_unlock(&lock);

}
