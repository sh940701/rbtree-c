#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

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


node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *node = (node_t *) calloc(1, sizeof(node_t));

    node->right = t->nil;
    node->left = t->nil;
    node->key = key;
    node->color = RBTREE_RED;

    // 트리가 비어있을 때
    if (t->root == t->nil) {
        node->color = RBTREE_BLACK;
        node->parent = t->nil;
        t->root = node;

        return node;
    }

    // 노드 넣기
    insert(t, node);

    node_t *target_node = node;

    while (1) {
        // case1 확인
        if (check_case1(t, target_node) == 1) {
            target_node = solve_case1(t, target_node);
            continue;
        }
        // case2 확인
        if (check_case2(t, target_node) != 0) {
            if (check_case2(t, target_node) == 1) {
                target_node = rotate_small_left(target_node);
                rotate_large_right(t, target_node);
            } else if (check_case2(t, target_node) == 2) {
                target_node = rotate_small_right(target_node);
                rotate_large_left(t, target_node);
            }
        }
            // case 3 확인
        else if (check_case3(t, target_node) != 0) {
            if (check_case3(t, target_node) == 1) {
                rotate_large_right(t, target_node);
            } else if (check_case3(t, target_node) == 2) {
                rotate_large_left(t, target_node);
            }
        }

        break;
    }

    return node;
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
    // TODO: implement find
    return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
