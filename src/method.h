#include "rbtree.h"

void insert(rbtree *, node_t *);

int check_insert_case1(rbtree *, node_t *);

node_t *solve_insert_case1(rbtree *, node_t *);

int check_insert_case2(rbtree *, node_t *);

int check_insert_case3(rbtree *, node_t *);

void rotate_large_right(rbtree *, node_t *);

void rotate_large_left(rbtree *, node_t *);

node_t *rotate_small_right(node_t *);

node_t *rotate_small_left(node_t *);
