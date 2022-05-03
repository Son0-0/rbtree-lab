#include "rbtree.h"
#include <stdio.h> // 제출시 삭제
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = nilNode;
  p->root->parent = nilNode;
  return p;
}

void delete_postorder(rbtree *t, node_t *cur) {
  if (cur == t->nil)
    return;
  delete_postorder(t, cur->left);
  delete_postorder(t, cur->right);
  cur->left = cur->right = cur->parent = NULL;
  free(cur);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_postorder(t, t->root);
  free(t->nil);
  free(t);
}

void right_rotate(rbtree *t, node_t *x) {
  // TODO: implement right rotate
  // y의 right 서브 트리를 x의 left 서브 트리로 만들어 주는 과정
  node_t *y = x->left;
  x->left = y->right;

  // y의 right 서브 트리의 루트 노드의 부모를 x로 연결시키는 과정
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  // y의 부모를 x의 부모로 설정해준다. 
  y->parent = x->parent;

  // x의 부모가 nil이면 루트 노드 이니 루트를 y로 설정
  if (x->parent == t->nil) {
    t->root = y;
  }
  // x가 왼쪽 자식이라면
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  // x가 오른쪽 자식이라면
  else {
    x->parent->right = y;
  }

  // 그 후 x를 y의 right로 설정한다.
  y->right = x;
  x->parent = y;
}

void left_rotate(rbtree *t, node_t *x) {
  // TODO: implement left rotate
  // y의 left 서브 트리를 x의 right 서브 트리로 만들어 주는 과정
  node_t *y = x->right;
  x->right = y->left;

  // y의 left 서브 트리의 루트 노드의 부모를 x로 연결시키는 과정
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  // y의 부모를 x의 부모로 설정해준다. 
  y->parent = x->parent;

  // x의 부모가 nil이면 루트 노드 이니 루트를 y로 설정
  if (x->parent == t->nil) {
    t->root = y;
  }
  // x가 왼쪽 자식이라면
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  // x가 오른쪽 자식이라면
  else {
    x->parent->right = y;
  }

  // 그 후 x를 y의 left로 설정한다.
  y->left = x;
  x->parent = y;
}

void rb_insert_fixup(rbtree *t, node_t *target) {
  // TODO: implement insert fixup
  node_t *uncle;

  while (target->parent->color == RBTREE_RED) {
    // 부모 노드가 왼쪽 자식일 때
    if (target->parent == target->parent->parent->left) {
      // 삼촌 노드는 부모->부모의 오른쪽 자식
      uncle = target->parent->parent->right;
      // CASE 1: 삼촌 노드의 색상이 레드일 때
      if (uncle->color == RBTREE_RED) {
        uncle->color = target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }
      // CASE 2: 삼촌 노드의 색상이 블랙일 때 tri
      else {
        if (target == target->parent->right) {
          target = target->parent;
          left_rotate(t, target);
        }
        // CASE 3: 삼촌 노드의 색상이 블랙일 때 모양이 linear
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        right_rotate(t, target->parent->parent);
      }
    }
    // 부모 노드가 오른쪽 자식일 때
    else {
      uncle = target->parent->parent->left;
      // CASE 1: 삼촌 노드의 색상이 레드일 때 
      if (uncle->color == RBTREE_RED) {
        uncle->color = target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }
      // CASE 2: 삼촌 노드의 색상이 블랙일 때 tri
      else {
        if (target == target->parent->left) {
          target = target->parent;
          right_rotate(t, target);
        }
        // CASE 3: 삼촌 노드의 색상이 블랙일 때 모양이 linear
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        left_rotate(t, target->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  node_t *find = t->root;
  node_t *pos_node = t->nil;

  while (find != t->nil) {
    pos_node = find;
    if (key < find->key) {
      find = find->left;
    } else {
      find = find->right;
    }
  }

  new_node->parent = pos_node;

  if (pos_node == t->nil) {
    t->root = new_node;
  } else if (key < pos_node->key) {
    pos_node->left = new_node;
  } else {
    pos_node->right = new_node;
  }

  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t->nil;

  /* 
    BST의 Insert와 동일한 과정을 거쳐서 새로운 노드를 삽입합니다.
    그 후 fixUP을 통해 색상을 변경해주는 과정을 진행합니다.
  */
  rb_insert_fixup(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t->root;

  while (temp != t->nil) {
    if (temp->key < key) {
      temp = temp->right;
    } else if (key < temp->key) {
      temp = temp->left;
    } else {
      return temp;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  
  while (cur->left != t->nil) {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;

  while (cur->right != t->nil) {
    cur = cur->right;
  }

  return cur;
}

void rb_erase_fixup(rbtree *t, node_t *target) {
  node_t *sibling;
  
  while (target != t->root && target->color == RBTREE_BLACK) {
    // target이 왼쪽 자식일 때
    if (target == target->parent->left) {
      sibling = target->parent->right;
      // CASE 1
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_rotate(t, target->parent);
        sibling = target->parent->right;
      }
      // CASE 2
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        // CASE 3
        if (sibling->right->color == RBTREE_BLACK) {
          sibling->left->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          right_rotate(t, sibling);
          sibling = target->parent->right;
        }
        // CASE 4
        sibling->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }
    // target이 오른쪽 자식일 때
    else {
      sibling = target->parent->left;
      // CASE 1
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
        sibling = target->parent->left;
      }
      // CASE 2
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        // CASE 3
        if (sibling->left->color == RBTREE_BLACK) {
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          left_rotate(t, sibling);
          sibling = target->parent->left;
        }
        // CASE 4
        sibling->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *tree_minimum(rbtree *t, node_t *cur) {
  node_t *temp = cur;
  while (temp->left != t->nil) {
    temp = temp->left;
  }
  return temp;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  node_t *x = t->nil;

  color_t y_original_color = y->color;

  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  else {
    y = tree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    }
    else {
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
    rb_erase_fixup(t, x);
  }

  return 0;
}

int make_array_inorder(node_t *nil, node_t *cur, key_t *result, int index) {
  if (cur != nil) {
    index = make_array_inorder(nil, cur->left, result, index);
    result[index++] = cur->key;
    index = make_array_inorder(nil, cur->right, result, index);
  }
  return index;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  make_array_inorder(t->nil, t->root, arr, 0);
  return 0;
}

void inorder(rbtree *t, node_t *cur) {
  if (cur == t->nil) {
    return;
  }

  inorder(t, cur->left);
  printf("%d (%d) / ", cur->key, cur->color);
  inorder(t, cur->right);
}
