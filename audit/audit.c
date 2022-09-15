#include <stdlib.h>
#include <stdio.h>

struct truc {
  int x;
  struct truc *a, *b;
};

struct truc* nouveau(int v, struct truc* a, struct truc* b) {
  struct truc* t = malloc(sizeof(struct truc));
  t->x = v;
  t->a = a;
  t->b = b;
  return t;
};

int est_dans (struct truc* t, int v) {

  if (v < t->x)
    return est_dans(t->b, v);
  else
    if (v > t->x)
      return est_dans(t->a, v);
    else
      return 1;

  if (t == NULL)
    return 0;

}

struct truc* en_plus (struct truc* t, int v) {

  if (!est_dans(t, v)) {

    if (t == NULL)
      return nouveau(v, NULL, NULL);

    if (v < t->x)
      t->b = en_plus(t->b, v);
    else
      t->a = en_plus(t->a, v);

    return t;

  }

  else
    return t;

}

void test_est_dans(struct truc* t, int v) {
  if (est_dans(t, v))
    printf("%i est dans t\n", v);
  else
    printf("%i n'est pas dans t\n", v);
}

int main (int argc, char* argv[]) {

  struct truc* t = nouveau(4, NULL, NULL);
  test_est_dans(t, 4);
  test_est_dans(t, 1);
  t = en_plus(t, 5);
  test_est_dans(t, 5);
  t = en_plus(t, 1);
  test_est_dans(t, 1);

  return 1;
}
