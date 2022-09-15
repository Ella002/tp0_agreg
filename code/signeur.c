#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef MANIP_H
  #include "manip_buffer.h"
  #define MANIP_H
#endif

#define LISTEN_BACKLOG 50
#define MAX_BUF_LEN 512

// treat_msg(msg, buffer, use_threads) parse le message,
// qui est soit "sign" soit "add X",
// et applique la fonction de manip_buffer correspondante a buffer
// NB: si use_threads est vrai, on appelle la fonction d'ajout multithreadee
void treat_msg(char* msg, buf_t* buffer, bool use_threads) {

  // sign message
  if (msg[0] == 's') {
    sign(buffer);
  }
  else {
    // add message
    int X = strtoul(msg+4, NULL, 10); // on recupere l'entier apres "add "
    if (!use_threads) {
      // sans threads, on appelle simplement la fonction d'ajout
      add_x(buffer, X);
    }
    else {
      // avec threads
      pthread_t id;
      // on rassemble les arguments de add_x dans une structure
      // parce que pthread_create prend les arguments sous forme void*
      add_x_params_t* args = malloc(sizeof(add_x_params_t));
      args->x = X;
      args->buf = buffer;
      // on cree un thread, qui va appeler add_x_threads
      // et ajouter la valeur au tampon
      pthread_create(&id, NULL, (void*) add_x_threads, (void*) args);
    }
  }
}

// server(port, buffer, use_threads) met en place le serveur ecoutant sur port,
// et recevant des messages indiquant comment mettre a jour buffer
void server (char* port, buf_t* buffer, bool use_threads) {

  int sock_fd, client_fd;
  int status;
  struct addrinfo hints;
  struct addrinfo *res, *rp;
  struct sockaddr client_addr;
  socklen_t client_addr_size;

  char msg[MAX_BUF_LEN+1];

  // on setup l'addrinfo
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // getaddrinfo nous donne une liste d'adresses possibles
  if (status = getaddrinfo(NULL, port, &hints, &res)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  printf("getaddrinfo: success\n");

  // on essaie toutes les adresses de res jusqu'à réussir à créer socket et bind
  for (rp = res; rp != NULL; rp = rp->ai_next) {

    sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock_fd == -1)
      // ca a echoue: on passe a l'adresse suivante
      continue;

    if (bind(sock_fd, rp->ai_addr, rp->ai_addrlen) == 0)
      // on a reussi a bind sur cette adresse, ie connecter le socket au port
      break;

    close(sock_fd);
  }

  if (rp == NULL) {
    // on a echoue sur toutes les adresses
    fprintf(stderr, "Could not bind\n");
    exit(1);
  }

  printf("Binding: success\n");

  // plus besoin du resultat de getaddrinfo
  freeaddrinfo(res);

  // on commence a ecouter sur le socket:
  // les connexions vont etre stockees dans une file de taille au plus LISTEN_BACKLOG,
  // en attendant d'etre traites
  if (listen(sock_fd, LISTEN_BACKLOG) == -1) {
    fprintf(stderr, "Could not listen\n");
    exit(1);
  }

  printf("Listening...\n");

  while(1) {

    // on accepte une connexion d'un client
    client_addr_size = sizeof(struct sockaddr);
    client_fd = accept(sock_fd, &client_addr, &client_addr_size);
    if (client_fd == -1) {
      fprintf(stderr, "Could not accept\n");
      exit(1);
    }

    // on reçoit son message
    int msg_len = recv(client_fd, &msg, MAX_BUF_LEN, 0);

    close(client_fd);

    msg[msg_len] = '\0';

    // on parse le message et on effectue l'action correspondante
    treat_msg(msg, buffer, use_threads);

  }

  // Fin du serveur
  close(sock_fd);

}

int main (int argc, char* argv[]) {

  if (argc < 4) {
    printf("usage: ./signeur <taille T> <port> <thread or nothread>\n");
    return -1;
  }

  int T = atoi(argv[1]);
  char* port = argv[2];

  bool use_threads = (argv[3][0] == 't');

  buf_t* buffer = init(T, use_threads);

  server("8080", buffer, use_threads);

  delete(buffer);

  return 1;

}
