#include "rbtree.h"

void insert(rbtree *t, node_t *node) {
    node_t *tmp_parent = t->root;

    while (1) {
        if (tmp_parent->key > node->key) {
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

int check_insert_case1(rbtree *t, node_t *node) {
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
node_t *solve_insert_case1(rbtree *t, node_t *node) {
    node_t *grandparent = node->parent->parent;

    grandparent->left->color = RBTREE_BLACK;
    grandparent->right->color = RBTREE_BLACK;
    grandparent->color = RBTREE_RED;

    return grandparent;
}

int check_insert_case2(rbtree *t, node_t *node) {
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

int check_insert_case3(rbtree *t, node_t *node) {
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

    return 0;
}

void rotate_large_right(rbtree *t, node_t *node) {
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

void rotate_large_left(rbtree *t, node_t *node) {
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

node_t *rotate_small_right(node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    parent->left = node->right;
    node->right = parent;

    node->parent = grandparent;
    parent->parent = node;

    grandparent->right = node;

    return parent;
}

node_t *rotate_small_left(node_t *node) {
    node_t *parent = node->parent;
    node_t *grandparent = parent->parent;

    parent->right = node->left;
    node->left = parent;

    node->parent = grandparent;
    parent->parent = node;

    grandparent->left = node;

    return parent;
}
