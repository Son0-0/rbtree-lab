#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void leftrotate(node_t *cur, rbtree *t) {
  node_t *temp = (node_t*)calloc(1, sizeof(node_t));
  
  temp = cur->right;
  cur->right = temp->left;

  if (temp->left != t->nil)
    temp->left->parent = cur;

  temp->parent = cur->parent;

  if (cur->parent == t->nil)
    t->root = temp;

  else if (cur == cur->parent->left)
    cur->parent->left = temp;

  else
    cur->parent->right = temp;

  temp->left = cur;
  cur->parent = temp;

}

void rightrotate(node_t *cur, rbtree *t) {
  node_t *temp = (node_t*)calloc(1, sizeof(node_t));

  temp = cur->left;
  cur->left = temp->right;

  if (temp->right != t->nil)
    temp->right->parent = cur;

  temp->parent = cur->parent;

  if (cur->parent == t->nil)
    t->root = temp;

  else if (cur == cur->parent->left)
    cur->parent->left = temp;

  else
    cur->parent->right = temp;

  temp->right = cur;
  cur->parent = temp;

}

void check_color(node_t *cur, rbtree *t) {
  // TODO: check new node's color and re-balancing tree
  printf("cur node key: %d color: %d\n", cur->key, cur->color);

  if (cur->parent != NULL) {
    printf("cur_node's parent: %d color: %d\n", cur->parent->key, cur->parent->color);
  }

  if (cur->parent != NULL) {
    while (cur->parent != NULL && cur->parent->color == RBTREE_RED) { // 부모 노드 색상이 레드 ->
      if (cur->parent->parent->left != t->nil && cur->parent == cur->parent->parent->left) {
        if (cur->parent->parent->right != t->nil && cur->parent->parent->right->color == RBTREE_RED) { // COLOR = RED
          cur->parent->color = RBTREE_BLACK;
          cur->parent->parent->right->color = RBTREE_BLACK;
          cur->parent->parent->color = RBTREE_RED;
          cur = cur->parent->parent;
        } else { // COLOR = BLACK
          if (cur == cur->parent->right) {
            cur = cur->parent;
            leftrotate(cur, t);
          }
          cur->parent->color = RBTREE_BLACK;
          cur->parent->parent->color = RBTREE_RED;
          rightrotate(cur->parent->parent, t);
        }
      }

      else {
        if (cur->parent->parent->left != t->nil && cur->parent->parent->left->color == RBTREE_RED) {
          cur->parent->color = RBTREE_BLACK;
          cur->parent->parent->left->color = RBTREE_BLACK;
          cur->parent->parent->color = RBTREE_RED;
          cur = cur->parent->parent;
        } else {
          if (cur->parent->left != t->nil && cur == cur->parent->left) {
            cur = cur->parent;
            rightrotate(cur, t);
          }
          cur->parent->color = RBTREE_BLACK;
          cur->parent->parent->color = RBTREE_RED;
          leftrotate(cur->parent->parent, t);
        }
      }
    }
  }

  t->root->color = RBTREE_BLACK; // 트리의 루트면 블랙
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  node_t *cur_node = (node_t*)calloc(1, sizeof(node_t));
  node_t *temp = (node_t*)calloc(1, sizeof(node_t));

  (*new_node).key = key;
  (*new_node).color = RBTREE_RED; // 제일 처음 노드 생성시 색상은 무조건 레드
  (*new_node).left = (*new_node).right = t->nil;
  // (*new_node).parent = NULL;

  cur_node = t->root;

  while (cur_node != t->nil) { // BST insert와 동일
    temp = cur_node;
    if (new_node->key <= cur_node->key) {
      cur_node = cur_node->left;
    } else {
      cur_node = cur_node->right;
    }
  }

  if (t->root == NULL) {
    t->root = new_node;
  } else if (new_node->key <= temp->key) {
    temp->left = new_node;
    new_node->parent = temp;
  } else {
    temp->right = new_node;
    new_node->parent = temp;
  }
  // new_node->parent = temp;
  check_color(new_node, t); // RBTree 색상 체크

  return t->root;
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

void preorder(node_t* root, rbtree *t) {
  if (root == t->nil) {
    return;
  }

  printf("%d (%d)", root->key, root->color);
  preorder(root->left, t);
  preorder(root->right, t);
}

void inorder(node_t* root, rbtree *t) {
  if (root == t->nil) {
    return;
  }

  inorder(root->left, t);
  printf("%d ", root->key);
  inorder(root->right, t);
}

void postorder(node_t* root, rbtree *t) {
  if (root == t->nil) {
    return;
  }

  postorder(root->left, t);
  postorder(root->right, t);
  printf("%d ", root->key);
}

void test_insert_single(const key_t key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);
  // assert(p->color == RBTREE_BLACK);  // color of root node should be black
#ifdef SENTINEL
  assert(p->left == t->nil);
  assert(p->right == t->nil);
  assert(p->parent == t->nil);
#else
  assert(p->left == NULL);
  assert(p->right == NULL);
  assert(p->parent == NULL);
#endif
  printf("pass2\n");
}

void main() {
  rbtree *t = new_rbtree();
  assert(t != NULL);

#ifdef SENTINEL
  assert(t->nil != NULL);
  assert(t->root == t->nil);
#else
  assert(t->root == NULL);
#endif
  printf("pass\n");
  
  test_insert_single(1024);

  int size = 0;
  scanf("%d", &size);

  for (int i = 0; i < size; i++) {
    int temp = 0;
    printf("insert key: ");
    scanf("%d", &temp);

    node_t *p = rbtree_insert(t, temp); // insert key to rbtree
  }

  inorder(t->root, t);
  puts("");
  preorder(t->root, t);
  puts("");
  postorder(t->root, t);
  puts("");

}
