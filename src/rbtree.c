#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

static node_t *find_min(const rbtree *t, node_t *node) {
    while (node->left != t->nil) {
        node = node->left;
    }
    return node;
}

static node_t *find_max(const rbtree *t, node_t *node) {
    while (node->right != t->nil) {
        node = node->right;
    }
    return node;
}

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    node_t *nil = (node_t *) calloc(1, sizeof(node_t));

    nil->color = RBTREE_BLACK;

    p->nil = nil;
    p->root = nil;
    return p;
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory



    free(t);
}

static void insert(rbtree *t, node_t *node) {
    node_t *tmp_parent = t->root;

    while (1) {
        if (tmp_parent->key >= node->key) {
            if (tmp_parent->left == t->nil) {
                tmp_parent->left = node;
                node->parent = tmp_parent;
                return;
            } else {
                tmp_parent = tmp_parent->left;
            }
        } else {
            if (tmp_parent->right == t->nil) {
                tmp_parent->right = node;
                node->parent = tmp_parent;
                return;
            } else {
                tmp_parent = tmp_parent->right;
            }
        }
    }
}

static int check_case1(rbtree *t, node_t *node) {
    node_t *parent;
    node_t *grandparent;
    node_t *uncle;

    if (node->parent != t->nil) {
        parent = node->parent;
    } else {
        return 0;
    }
    if (parent->parent != t->nil) {
        grandparent = parent->parent;
    } else {
        return 0;
    }
    if (parent->key <= grandparent->key) {
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }

    if (node->color == RBTREE_RED && parent->color == RBTREE_RED && uncle->color == RBTREE_RED) {
        return 1;
    } else {
        return 0;
    }
}

// 조부모와 부모/삼촌 의 색깔을 바꿔주며, 이후 target 을 조부모 노드로 하여 반환
static node_t *solve_case1(rbtree *t, node_t *node) {
    node_t *grandparent = node->parent->parent;

    grandparent->left->color = RBTREE_BLACK;
    grandparent->right->color = RBTREE_BLACK;
    grandparent->color = RBTREE_RED;

    return grandparent;
}

static int check_case2(rbtree *t, node_t *node) {
    node_t *parent;
    node_t *grandparent;
    node_t *uncle;

    if (node->parent != t->nil) {
        parent = node->parent;
    } else {
        return 0;
    }
    if (parent->parent != t->nil) {
        grandparent = parent->parent;
    } else {
        return 0;
    }
    if (parent->key <= grandparent->key) {
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }
    if (parent->color == RBTREE_RED && uncle->color == RBTREE_BLACK) {
        if (grandparent->left == parent && parent->right == node) {
            return 1; // parent 가 grandparent 의 왼쪽이고 node 가 parent 의 오른쪽인 경우 1 반환
        } else if (grandparent->right == parent && parent->left == node) {
            return 2; // parent 가 grandparent 의 오른쪽이고 node 가 parent 의 왼쪽인 경우 1 반환
        }
    }

    return 0;
}

static int check_case3(rbtree *t, node_t *node) {
    node_t *parent;
    node_t *grandparent;
    node_t *uncle;

    if (node->parent != t->nil) {
        parent = node->parent;
    } else {
        return 0;
    }
    if (parent->parent != t->nil) {
        grandparent = parent->parent;
    } else {
        return 0;
    }
    if (parent->key <= grandparent->key) {
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }

    if (parent->color == RBTREE_RED && uncle->color == RBTREE_BLACK) {
        if (grandparent->left == parent && parent->left == node) {
            return 1; // 오른쪽으로 회전 필요
        } else if (grandparent->right == parent && parent->right == node) {
            return 2; // 왼쪽으로 회전 필요
        }
    }
}

static void rotate_large_right(rbtree *t, node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    int is_root = t->root == grandparent;

    parent->color = RBTREE_BLACK;
    grandparent->color = RBTREE_RED;

    grandparent->left = parent->right;
    parent->right = grandparent;

    parent->parent = grandparent->parent;
    if (is_root == 0) {
        if (grandparent->key <= grandparent->parent->key) {
            grandparent->parent->left = parent;
        } else {
            grandparent->parent->right = parent;
        }
    } else {
        t->root = parent;
    }
    grandparent->parent = parent;
}

static void rotate_large_left(rbtree *t, node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    int is_root = t->root == grandparent;

    parent->color = RBTREE_BLACK;
    grandparent->color = RBTREE_RED;

    grandparent->right = parent->left;
    parent->left = grandparent;

    parent->parent = grandparent->parent;
    if (is_root == 0) {
        if (grandparent->key <= grandparent->parent->key) {
            grandparent->parent->left = parent;
        } else {
            grandparent->parent->right = parent;
        }
    } else {
        t->root = parent;
    }
    grandparent->parent = parent;
}

static node_t *rotate_small_right(node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    parent->left = node->right;
    node->right = parent;

    node->parent = grandparent;
    parent->parent = node;

    grandparent->right = node;

    return parent;
}

static node_t *rotate_small_left(node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    parent->right = node->left;
    node->left = parent;

    node->parent = grandparent;
    parent->parent = node;

    grandparent->left = node;

    return parent;
}

void print(rbtree *t, node_t *node) {
    if (node->left != t->nil) {
        print(t, node->left);
    }

    if (node->color == 0) {
        printf("key: %d, color: %s\n", node->key, "Red");
    } else {
        printf("key: %d, color: %s\n", node->key, "Black");
    }

    if (node->right != t->nil) {
        print(t, node->right);
    }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    if (t->root == t->nil) {
        return NULL;
    }

    node_t *node = t->root;

    while (node != t->nil) {
        if (node->key == key) {
            break;
        } else if (node->key > key) {
            node = node->left;
        } else if (node->key < key) {
            node = node->right;
        }
    }

    if (node == t->nil) {
        node = NULL;
    }

    return node;
}

node_t *rbtree_min(const rbtree *t) {
    if (t->root == t->nil) {
        return t->nil;
    }
    node_t *min_node = find_min(t, t->root);
    return min_node;
}

node_t *rbtree_max(const rbtree *t) {
    if (t->root == t->nil) {
        return t->nil;
    }
    node_t *max_node = find_max(t, t->root);
    return max_node;
}

static void to_array(const rbtree *t, node_t *node, key_t *arr, int *idx) {
    if (node->left != t->nil) {
        to_array(t, node->left, arr, idx);
    }

    arr[*idx] = node->key;
    *idx += 1;

    if (node->right != t->nil) {
        to_array(t, node->right, arr, idx);
    }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    int idx = 0;

    to_array(t, t->root, arr, &idx);

    return 0;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(rbtree *t, node_t *y) {
    node_t *x = y->left;
    y->left = x->right;
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;

    if (y->parent == t->nil) {
        t->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
    node_t *w;
    while (x != t->root && x->color == RBTREE_BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotate(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotate(t, x->parent);
                x = t->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }

    x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
    node_t *y = z;
    color_t y_original_color = z->color;
    node_t *x;

    if (z->left == t->nil) {
        x = z->right;
        rb_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rb_transplant(t, z, z->left);
    } else {
        y = find_min(t, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rb_transplant(t, z, y);

        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == RBTREE_BLACK) {
        rb_delete_fixup(t, x);
    }

    free(z);

    return 0;
}

void insert_fixup(rbtree *t, node_t *x) {

    while (x->parent->color == RBTREE_RED) {
        // 부모랑 왼쪽 삼촌이 같은 즉 1자로 된 그래프의 경우의 수
        if (x->parent == x->parent->parent->left) {
            node_t *right_uncle = x->parent->parent->right;
            // 만약 부모의 색깔이 레드인 경우, 색깔을 바꿔줌
            if (right_uncle->color == RBTREE_RED) {
                // 맨위가 블랙, 그 왼쪽,오른쪽이 레드 왼쪽의 왼쪽이 레드로 삽입된 케이스
                // 그러면 맨위의 왼쪽오른쪽이 다 레드이고 맨위가 블랙이라서 색깔을 바꿔주면 되는 케이스가 됨
                x->parent->color = RBTREE_BLACK;
                right_uncle->color = RBTREE_BLACK;
                x->parent->parent->color = RBTREE_RED;

                // 바꾼 이후로는 x의값(포인터이기 때문에 주소만 들고있음)을 할아버지 값으로 옮겨서 x에 대해서 겁사를 또 할 수 있도록 만듦
                //  따라서 새로 삽입된 곳에 할아버지를 넣는게 아니라 기존 주소 대신 할아버지 주소를 받아서
                //  while문을 통해서 체크하는 것임
                x = x->parent->parent;
            } else {
                // 양쪽 왼쪽 오른쪽 값에 누군가 있었다면? 가능한가?
                if (x == x->parent->right) {
                    // 맨 위 노드가 레드이고 맨위의 왼쪽이 블랙, 오른쪽이 원래 x값일 때의 경우의수
                    // 알고리즘책의 319페이지
                    // 비교대상을 x의 부모로 바꿔줘서 이상이 있는지를 체크
                    // 다만 여기에 들어왔단것은 문제가 있단것이므로 부모를기준으로 왼쪽으로 돌려야함
                    x = x->parent;

                    left_rotate(t, x);
                }
                x->parent->color = RBTREE_BLACK;
                // grandfather가 안되는 이유는 이 위에서 x값을 바꿨기 때문에 바뀐 상태에서 이리로 오면 parent값이 되어버림
                x->parent->parent->color = RBTREE_RED;
                right_rotate(t, x->parent->parent);
            }
        }
            // 부모랑 오른 삼촌이 같은 즉 1자로 된 그래프의 경우의 수
            // left->right로 변경
        else {
            node_t *left_uncle = x->parent->parent->left;
            if (left_uncle->color == RBTREE_RED) {
                x->parent->color = RBTREE_BLACK;
                x->parent->parent->color = RBTREE_RED;
                left_uncle->color = RBTREE_BLACK;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    // 꺾임
                    x = x->parent;
                    right_rotate(t, x);
                }
                x->parent->color = RBTREE_BLACK;
                x->parent->parent->color = RBTREE_RED;

                left_rotate(t, x->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // 인서트의 노드가 n개인 레드블랙 트리의 높이는 O(log N) 이므로
    //  인서트의 수행시간은 O(logN)이 된다.
    node_t *y = t->nil;
    node_t *x = t->root;
    // x(t-<root)가 t->nil값이라면 값이 없는거니까 그냥 바로 집어넣으면 됨
    // while문을 통해서 x값을 바꾸면서 값을 찾아가야함

    while (x != t->nil) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node_t *z = (node_t *) calloc(1, sizeof(node_t));
    z->parent = y;
    z->key = key;

    // 아예 부모가 없는 경우 즉 while문을 타지 않았을 경우에 해당
    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    // 새로운 노드값에 대해서 삽입을 해야하니까
    // z값을 초기화 시켜줌
    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;
    // 삽입할 때 색깔은 항상 레드이며 삽입 이후에는 fixup을 이용해야함
    insert_fixup(t, z);
    return t->root;
}
