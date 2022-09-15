typedef struct t {
  unsigned int* tab;
  int ptr;
  int taille;
} buf_t;

typedef struct u {
  buf_t* buf;
  int x;
} add_x_params_t;

buf_t* init(int, bool);
void sign (buf_t*);
void add_x (buf_t*, unsigned int);
void delete(buf_t*);
void print(buf_t*);
void add_x_threads(void*);
