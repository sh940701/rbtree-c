#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef int key_t;

typedef struct node_t {
    key_t key;
    struct node_t *parent, *left, *right;
} node_t;

typedef struct {
    node_t *root;
} bstree;


bstree *new_bstree(void) {
    bstree *p = (bstree *) calloc(1, sizeof(bstree));
    p->root = NULL;
    return p;
}

void delete_bstree(bstree *t) {
    // TODO: reclaim the tree nodes's memory
    free(t);
}

// bstree 의 root 를 받는다.
node_t *bstree_insert(node_t *node, node_t *parent, const key_t key) {
    if (node == NULL) {
        node_t *p = (node_t *) calloc(1, sizeof(node_t));

        p->parent = parent;
        p->left = NULL;
        p->right = NULL;
        p->key = key;

        // root 노드인 경우, parent 가 NULL 이므로 여기서 바로 리턴
        if (parent == NULL) {
            return p;
        }

        if (parent->key < key) {
            parent->right = p;
        } else if (parent->key > key) {
            parent->left = p;
        }

        return p;
    } else if (node->key < key) {
        return bstree_insert(node->right, node, key);
    } else if (node->key > key) {
        return bstree_insert(node->left, node, key);
    }

    // 이미 키가 존재하는 경우, 추가하지 않음
    return node;
}


// bstree 의 root 를 받는다.
const node_t *bstree_find(const node_t *node, const key_t key) {
    if (node->key == key) {
        return node;
    } else if (node->key < key) {
        if (node->right != NULL) {
            return bstree_find(node->right, key);
        }
    } else if (node->key > key) {
        if (node->left != NULL) {
            return bstree_find(node->left, key);
        }
    }
}

// bstree 의 root 를 받는다.
node_t *bstree_min(node_t *node) {
    if (node->left == NULL) {
        return node;
    } else {
        return bstree_min(node->left);
    }
}

node_t *bstree_max(node_t *node) {
    if (node->right == NULL) {
        return node;
    } else {
        return bstree_max(node->right);
    }
}

int bstree_erase(node_t *node) {
    // 삭제하는 노드가 리프 노드인 경우
    if (node->right == NULL && node->left == NULL) {
        if (node->key > node->parent->key) {
            node->parent->right = NULL;
        } else {
            node->parent->left = NULL;
        }
    }

        // 삭제하는 노드의 자식 노드가 1개일 경우
    else if (node->right == NULL) { // left 노드만 있는 경우
        if (node->key > node->parent->key) { // 현재 노드가 부모 노드의 right 노드인 경우
            node->parent->right = node->left;
        } else {
            node->parent->left = node->left;
        }
        node->left->parent = node->parent;
    } else if (node->left == NULL) { // right 노드만 있는 경우
        if (node->key > node->parent->key) { // 현재 노드가 부모 노드의 right 노드인 경우
            node->parent->right = node->right;
        } else {
            node->parent->left = node->right;
        }
        node->right->parent = node->parent;
    }

        // 삭제하는 노드의 자식 노드가 2개일 경우
    else {
        node_t *successor = bstree_min(node->right);

        // 새 노드를 부모의 자식 노드로 추가
        if (node->key > node->parent->key) {
            node->parent->right = successor;
        } else {
            node->parent->left = successor;
        }
        successor->parent = node->parent; // 부모 노드를 새 노드의 부모로 추가

        // 자식 노드를 새 노드의 자식 노드로 추가
        successor->left = node->left;
        successor->right = node->right;

        // 새 노드를 자식 노드의 부모 노드로 추가
        node->left->parent = successor;
        node->right->parent = successor;
    }
    free(node);
    return 0;
}

void inorder(node_t *node, key_t *arr, int *idx) {
    if (node->left != NULL) {
        inorder(node->left, arr, idx);
    }
    arr[*idx] = node->key;
    *idx += 1;
    if (node->right != NULL) {
        inorder(node->right, arr, idx);
    }
}

void bstree_to_array(const bstree *t, key_t *arr, const size_t n) {
    int idx = 0;
    inorder(t->root, arr, &idx);
}

int main() {
    bstree *tree = new_bstree();

    // 10, 20, 30 insert
    tree->root = bstree_insert(tree->root, NULL, 20);
    node_t *node1 = bstree_insert(tree->root, NULL, 10);
    node_t *node2 = bstree_insert(tree->root, NULL, 30);

    printf("%p\n", node1);

    // insert 확인
    assert(tree->root->key == 20);
    printf("%d\n", tree->root->key);
    assert(tree->root->left->key == 10);
    printf("%d\n", tree->root->left->key);
    assert(tree->root->right->key == 30);
    printf("%d\n", tree->root->right->key);

    // find 확인
    node_t *root_find = bstree_find(tree->root, tree->root->key);
    node_t *node1_find = bstree_find(tree->root, node1->key);
    node_t *node2_find = bstree_find(tree->root, node2->key);

    printf("%p\n", node1);

    assert(root_find == tree->root);
    printf("%d\n", root_find == tree->root);
    assert(node1_find == node1);
    printf("%d\n", node1_find == node1);
    assert(node2_find == node2);
    printf("%d\n", node2_find == node2);

//    printf("%p\n", &node1_find);
//    printf("%p\n", &node2_find);

    // erase 확인
//    bstree_erase(tree->root);
    bstree_erase(node1);
    bstree_erase(node2);

    printf("%p\n", node1);

    const node_t *node1_find_after_erase = bstree_find(tree->root, node1->key);
    const node_t *node2_find_after_erase = bstree_find(tree->root, node2->key);

    printf("%p\n", node1_find_after_erase);



//    printf("%p\n", node1_find_after_erase);
//    printf("%p\n", node1);
//    printf("%p\n", node2_find_after_erase);
//    printf("%p\n", node2);

    assert(root_find == tree->root);
    printf("%d\n", root_find == tree->root);
    assert(node1_find_after_erase != node1);
    printf("%d\n", node1_find_after_erase == node1);
    assert(node2_find_after_erase != node2);
    printf("%d\n", node2_find_after_erase == node2);

//    node_t *node1 = bstree_insert(tree->root, NULL, 20);
//
//    node_t *node2 = bstree_insert(tree->root, NULL, 10);
//
//    node_t *node3 = bstree_insert(tree->root, NULL, 30);
//
//    printf("%d", node1->key);
//    printf("%d", node2->key);
//    printf("%d", node3->key);
//
//
//    bstree_erase(node1);
//    bstree_erase(node2);
//    bstree_erase(node3);



//    node_t *node1 = bstree_find(tree->root, 10);
//    node_t *node2 = bstree_find(tree->root, 20);
//    node_t *node3 = bstree_find(tree->root, 30);
//
//
//    printf("%d", node1->key);
//    printf("%d", node2->key);
//    printf("%d", node3->key);
//
//    bstree_erase(node1);

    key_t arr[100];

    bstree_to_array(tree, arr, 0);

    for (int i = 0; i < 3; i++) {
        printf("%d ", arr[i]);
    }

    delete_bstree(tree);


}
