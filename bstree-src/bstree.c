#include "bstree.h"

#include <stdlib.h>
#include <stdio.h>

bstree *new_bstree(void) {
    bstree *p = (bstree *) calloc(1, sizeof(bstree));

    p->root = NULL;
    return p;
}

void delete_bstree(bstree *t) {
    // TODO: reclaim the tree nodes' memory
    free(t);
}

node_t *bstree_insert(bstree *t, const key_t key) {
    node_t *node = t->root;

    // root node 가 NULL 일 때
    if (node == NULL) {
        node = (node_t *) calloc(1, sizeof(node_t));

        node->parent = NULL;
        node->key = key;
        node->left = NULL;
        node->right = NULL;

        t->root = node;

        return node;
    }


    node_t *tmp_parent = NULL;

    while (node != NULL) {
        if (node->key > key) {
            tmp_parent = node;
            node = node->left;
        } else if (node->key < key) {
            tmp_parent = node;
            node = node->right;
        } else if (node->key == key) {
            return NULL;
        }
    }

    node = (node_t *) calloc(1, sizeof(node_t));

    node->parent = tmp_parent;
    node->key = key;
    node->left = NULL;
    node->right = NULL;

    if (tmp_parent != NULL) {
        if (node->parent->key > key) {
            node->parent->left = node;
        } else if (node->parent->key < key) {
            node->parent->right = node;
        }
    }

    return node;
}

node_t *bstree_find(const bstree *t, const key_t key) {
    if (t->root == NULL) {
        return NULL;
    }

    printf("%d\n", key);

    node_t *node = t->root;

    while (node != NULL) {
        if (node->key == key) {
            break;
        } else if (node->key > key) {
            node = node->left;
        } else if (node->key < key) {
            node = node->right;
        }
    }

    return node;
}

static node_t *find_min(node_t *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

node_t *bstree_min(const bstree *t) {
    if (t->root == NULL) {
        return NULL;
    }
    node_t *min_node = find_min(t->root);
    return min_node;
}

static node_t *find_max(node_t *node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

node_t *bstree_max(const bstree *t) {
    if (t->root == NULL) {
        return NULL;
    }
    node_t *max_node = find_max(t->root);
    return max_node;
}

int bstree_erase(bstree *t, node_t *p) {
    int is_root = t->root == p;
    // 자식 노드가 하나도 없을 때
    if (p->left == NULL && p->right == NULL) {
        // p 의 parent 가 있을 때
        if (p->parent != NULL) {
            if (p->parent->key > p->key) {
                p->parent->left = NULL;
            } else {
                p->parent->right = NULL;
            }
            p->parent = NULL;
        }
        if (is_root == 1) {
            t->root = NULL;
        }
    }
        // 자식 노드가 하나만 있을 때
    else if (p->left == NULL) { // 오른쪽 노드만 있는 경우
        p->right->parent = p->parent;
        if (p->parent != NULL) { // 부모가 있을 때
            if (p->parent->key > p->key) {
                p->parent->left = p->right;
            } else {
                p->parent->right = p->right;
            }
        }
        if (is_root == 1) {
            t->root = p->right;
        }
    } else if (p->right == NULL) { // 왼쪽 노드만 있을 때
        p->left->parent = p->parent;
        if (p->parent != NULL) {
            if (p->parent->key > p->key) {
                p->parent->left = p->left;
            } else {
                p->parent->right = p->left;
            }
        }
        if (is_root == 1) {
            t->root = p->left;
        }
    }
        // 자식 노드가 두 개일 때
    else {
        node_t *change_target_node = find_min(p->right);

        change_target_node->left = p->left;
        p->left->parent = change_target_node;
        if (p->right != change_target_node) {
            if (change_target_node->right != NULL) {
                change_target_node->right->parent = change_target_node->parent;
            }
            change_target_node->parent->left = change_target_node->right;
            change_target_node->right = p->right;
            p->right->parent = change_target_node;
        }

        change_target_node->parent = p->parent;
        if (p->parent != NULL) {
            if (p->parent->key > p->key) {
                p->parent->left = change_target_node;
            } else {
                p->parent->right = change_target_node;
            }
        }

        if (is_root == 1) {
            t->root = change_target_node;
        }
    }

    free(p);
    return 0;
}

static void to_array(node_t *node, key_t *arr, int *idx) {
    if (node->left != NULL) {
        to_array(node->left, arr, idx);
    }

    arr[*idx] = node->key;
    *idx += 1;

    if (node->right != NULL) {
        to_array(node->right, arr, idx);
    }
}

int bstree_to_array(const bstree *t, key_t *arr, const size_t n) {
    int idx = 0;

    to_array(t->root, arr, &idx);

    return 0;
}
