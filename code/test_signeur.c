#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// send_message(address, port, msg, msg_len) envoie msg, de taille msg_length
// via un socket connecte a address sur port
void send_message (char* address, char* port, char* msg, int msg_len) {

  int sock_fd;
  struct addrinfo hints;
  struct addrinfo *res, *rp;

  // on setup l'addrinfo
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  // getaddrinfo nous donne une liste d'adresses possibles
  if (getaddrinfo(address, port, &hints, &res)) {
    fprintf(stderr, "Could not getaddrinfo.\n");
    exit(1);
  }

  // on essaie toutes les adresses de res jusqu'a reussir a socket et connect
  for (rp = res; rp != NULL; rp = rp->ai_next) {
    sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (sock_fd == -1)
      continue;

    // on se connecte au serveur
    if (connect(sock_fd, rp->ai_addr, rp->ai_addrlen) != -1) {
      fprintf(stdout, "Connected.\n");
      break;
    }

    close(sock_fd);
  }

  if (rp == NULL) {
    fprintf(stderr, "Could not connect\n");
    exit(1);
  }

  freeaddrinfo(res);

  // on envoie le message par le socket
  send(sock_fd, msg, msg_len, 0);

  // on ferme le socket
  close(sock_fd);

}

int main (int argc, char* argv[]) {

  if (argc < 5) {
    printf("usage: ./test_signeur <nb cmd> <proportion add/sign> <ip> <port>\n");
    return -1;
  }

  int nb_cmd = atoi(argv[1]);
  int ratio = atoi(argv[2]);
  char* ip = argv[3];
  char* port = argv[4];

  int cpt, a, X;
  char msg[32];

  srand(time(NULL));

  // on veut envoyer nb_cmd, donc ratio/100 sont des add
  for (cpt = 0; cpt < nb_cmd; cpt++) {
    // si a < ratio, on envoie add, sinon on envoie sign
    a = rand() % 100;
    if (a < ratio) {
      // envoi d'une requete add, X = entier aleatoire (max 32 pour les tests)
      X = rand() % 32;
      printf("adding, X = %i\n", X);
      sprintf(msg, "add %d", X);
      send_message(ip, port, msg, strlen(msg));
    }
    else {
      // envoi d'une requete sign
      printf("signing\n");
      send_message(ip, port, "sign", 4);
    }
  }

  return 0;

}
