#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Structure d'un arbre binaire de recherche: (cle, sag, sad).
   L'element "cle" est dans l'arbre.
   Tout element de cle < "cle" (resp. > "cle")
   est dans le sous-arbre gauche (resp. droit)
*/
typedef struct t {
  int cle;
  struct t *sad, *sag;
} abr_t;

/* On definit l'arbre vide comme le pointeur NULL */
abr_t* arbre_vide = NULL;

bool est_vide(abr_t* abr) {
  return (abr == arbre_vide);
}

/* cree(x, gauche, droit) cree un nouvel arbre, de cle x,
   avec gauche (resp. droit) comme sous-arbre gauche (resp. droit)
*/
abr_t* cree_arbre(int x, abr_t* gauche, abr_t* droit) {
  // on alloue l'espace memoire pour l'abr
  abr_t* abr = malloc(sizeof(abr_t));
  // on initialise les attributs
  abr->cle = x;
  abr->sag = gauche;
  abr->sad = droit;
  return abr;
};

/* trouve_cle(abr, x) cherche par recursion si la cle x est dans abr */
int trouve_cle(abr_t* abr, int x) {

  // si l'arbre est vide, elle n'y est pas
  if (abr == NULL) {
    return 0;
  }

  // si c'est la cle de t, on s'arrete
  if (abr->cle == x) {
    return 1;
  }

  // sinon, si x < abr->cle, on cherche dans le sous-arbre gauche
  if (x < abr->cle) {
    return trouve_cle(abr->sag, x);
  }
  else
    // sinon, x > abr->cle, on cherche dans le sous-arbre droit
    return trouve_cle(abr->sad, x);

}

/* ajoute_cle(abr, x) ajoute la cle x a abr, si elle n'y est pas deja */
abr_t* ajoute_cle(abr_t* abr, int x) {

  // si la cle n'est pas dans l'abr
  if (!trouve_cle(abr, x)) {

    // si l'abr est vide, on cree un nouvel arbre avec la cle
    if (est_vide(abr)) {
      return cree_arbre(x, arbre_vide, arbre_vide);
    }

    // sinon, si x < abr->cle, on ajoute la cle dans le sous-arbre gauche
    if (x < abr->cle) {
      abr->sag = ajoute_cle(abr->sag, x);
    }
    // sinon, x > abr->cle, on ajoute la cle dans le sous-arbre droit
    else {
      abr->sad = ajoute_cle(abr->sad, x);
    }

    return abr;

  }

  else {
    return abr;
  }

}

/* ajoute_cle_optim(abr, x) ajoute la cle x a abr, si elle n'y est pas deja,
   en parcourant l'arbre pour simultanement chercher si x y est, et trouver
   ou l'ajouter si non
*/
abr_t* ajoute_cle_optim(abr_t* abr, int x) {

  // si l'arbre est vide, on cree un nouvel arbre avec la clee
  if (est_vide(abr)) {
    return cree_arbre(x, arbre_vide, arbre_vide);
  }

  // sinon, si x < abr->cle, on ajoute la cle dans le sous-arbre gauche
  if (x < abr->cle) {
    abr->sag = ajoute_cle_optim(abr->sag, x);
  }
  else {
    if (x > abr->cle) {
      // sinon, si x > abr->cle, on ajoute la cle dans le sous-arbre droit
      abr->sad = ajoute_cle_optim(abr->sad, x);
    }
  }

  // on retourne l'abr, non modifie si x == abr->cle
  return abr;

}

/* detruire_arbre(abr) detruit l'arbre abr, en liberant les pointeurs */
void detruire_arbre(abr_t* abr) {

  if (!est_vide(abr)) {
    detruire_arbre(abr->sag);
    detruire_arbre(abr->sad);
    free(abr);
  }

}

void test_est_vide(abr_t* abr) {
  if (est_vide(abr))
    printf("L'abr est vide\n");
  else
    printf("L'abr n'est pas vide\n");
}

void test_trouve_cle(abr_t* abr, int x) {
  if (trouve_cle(abr, x))
    printf("%i est dans l'abr\n", x);
  else
    printf("%i n'est pas dans l'abr\n", x);
}

int main (int argc, char* argv[]) {

  // tests de base
  abr_t* abr_test = cree_arbre(4, NULL, NULL);
  test_trouve_cle(abr_test, 4);
  test_trouve_cle(abr_test, 1);
  abr_test = ajoute_cle(abr_test, 5);
  test_trouve_cle(abr_test, 5);
  abr_test = ajoute_cle(abr_test, 1);
  test_trouve_cle(abr_test, 1);

  // tests ajout optimise
  printf("-----------------------------\nTests avec ajout optimise\n");
  abr_t* abr_test2 = cree_arbre(4, NULL, NULL);
  test_trouve_cle(abr_test2, 4);
  test_trouve_cle(abr_test2, 1);
  abr_test2 = ajoute_cle_optim(abr_test2, 5);
  test_trouve_cle(abr_test2, 5);
  abr_test2 = ajoute_cle_optim(abr_test2, 1);
  test_trouve_cle(abr_test2, 1);
  test_trouve_cle(abr_test2, 4);
  test_trouve_cle(abr_test2, 5);

  // test destruction
  printf("-----------------------------\nTest destruction\n");
  detruire_arbre(abr_test);
  //printf("%i\n", abr_test->cle);

  //plus de tests
  printf("-----------------------------\nPlus de tests\n");
  abr_t* abr_test3 = arbre_vide;
  printf("On a bien un arbre vide: ");
  test_est_vide(abr_test3);
  abr_test3 = ajoute_cle(abr_test3, 4);
  printf("On a ajoute 4, on a bien un arbre qui n'est plus vide: ");
  test_est_vide(abr_test3);
  printf("qui contient 4: ");
  test_trouve_cle(abr_test3, 4);
  printf("et qui ne contient pas 5: ");
  test_trouve_cle(abr_test3, 5);
  abr_test3 = ajoute_cle(abr_test3, 4);
  printf("On a a nouveau ajoute 4, 4 est toujours dans l'abr: ");
  test_trouve_cle(abr_test3, 4);
  printf("Les sous-arbres de notre abr sont vides, on n'a pas ajoute 4 en double:\n");
  test_est_vide(abr_test3->sag);
  test_est_vide(abr_test3->sad);

  detruire_arbre(abr_test2);
  detruire_arbre(abr_test3);
  return 1;
}
