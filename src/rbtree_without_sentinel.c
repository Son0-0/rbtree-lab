#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

node_t *create_node(void) {
  // TODO: create node 
  node_t *temp = (node_t*)calloc(1, sizeof(node_t));
  return temp;
}

void pdelete(node_t* root) {
  if (root == NULL) {
    return;
  }
  pdelete(root->left);
  pdelete(root->right);
  root->left = root->right = NULL;
  free(root);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *temp = t->root;

  pdelete(temp);
  free(t);
}

void rightrotate(node_t *cur, rbtree *t) {
  if (cur->left != NULL) {
    node_t *temp = cur->left;
    cur->left = temp->right;

    if (temp->right != NULL)
      temp->right->parent = cur;
    temp->parent = cur->parent;

    if (cur->parent == NULL)
      t->root = temp;
    else if (cur == cur->parent->left)
      cur->parent->left = temp;
    else
      cur->parent->right = temp;

    temp->right = cur;
    cur->parent = temp;
  }
}

void leftrotate(node_t *cur, rbtree *t) {
  if (cur->right != NULL) {
    node_t *temp = cur->right;
    cur->right = temp->left;

    if (temp->left != NULL) {
      temp->left->parent = cur;
    }
    temp->parent = cur->parent;

    if (cur->parent == NULL) {
      t->root = temp;
    }
    else if (cur == cur->parent->left) {
      cur->parent->left = temp;
    }
    else {
      cur->parent->right = temp;
    }

    temp->left = cur;
    cur->parent = temp;
  }
}

void check_color(node_t *cur, rbtree *t) {
  // TODO: check new node's color and re-balancing tree
  // 부모 노드 색상이 레드라면 계속 색상 변경 필요
  while (cur->parent && cur->parent->parent && cur->parent->color == RBTREE_RED) {
    // UNCLE이 PP의 RIGHT 경우
    if (cur->parent == cur->parent->parent->left) {
      node_t *temp = cur->parent->parent->right;
      // IF UNCLE COLOR = RED
      if (temp && temp->color == RBTREE_RED) { 
        cur->parent->color = RBTREE_BLACK;
        temp->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        cur = cur->parent->parent;
      }
      // ELSE UNCLE COLOR = BLACK
      else { 
        // TRI 구조인 경우 LEFT 회전 후 RIGHT 회전
        if (cur == cur->parent->right) {
          cur = cur->parent;
          leftrotate(cur, t);
        }
        if (cur->parent) {
          // LINEAR 구조인 경우 바로 RIGHT 회전
          cur->parent->color = RBTREE_BLACK;
          if (cur->parent->parent) {
            cur->parent->parent->color = RBTREE_RED;
            rightrotate(cur->parent->parent, t);
          }
        }
      }
    }

    // UNCLE이 PP의 LEFT인 경우
    else { 
      node_t *temp = cur->parent->parent->left;
      // IF UNCLE COLOR = RED
      if (temp && temp->color == RBTREE_RED) {
        cur->parent->color = RBTREE_BLACK;
        temp->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        cur = cur->parent->parent;
      } 
      // ELSE UNCLE COLOR = BLACK
      else { 
        // TRI 구조인 경우 LEFT 회전 후 RIGHT 회전
        if (cur == cur->parent->left) {
          cur = cur->parent;
          rightrotate(cur, t);
        }
        // LINEAR 구조인 경우 바로 RIGHT 회전
        if (cur->parent) {
          cur->parent->color = RBTREE_BLACK;
          if (cur->parent->parent) {
            cur->parent->parent->color = RBTREE_RED;
            leftrotate(cur->parent->parent, t);
          }
        }
      }
    }
  }

  // 트리의 루트면 블랙
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = create_node();
  // 제일 처음 노드 생성시 색상은 무조건 레드
  new_node->key = key;
  new_node->color = RBTREE_RED; 
  node_t *cur_node = t->root;
  node_t *temp = NULL;

  // BST insert와 동일
  while (cur_node != NULL) { 
    temp = cur_node;
    if (new_node->key < cur_node->key) {
      cur_node = cur_node->left;
    } else {
      cur_node = cur_node->right;
    }
  }

  new_node->parent = temp;
  if (temp == NULL) {
    t->root = new_node;
  }
  else if (new_node->key < temp->key) {
    temp->left = new_node;
  }
  else {
    temp->right = new_node;
  }

  // RBTree 색상 체크
  check_color(new_node, t);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t->root;

  while (temp != NULL) {
    if (temp->key < key)
      temp = temp->right;

    else if (key < temp->key)
      temp = temp->left;
    
    else
      return temp;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t->root;

  while (temp->left != NULL) {
    temp = temp->left;
  }
  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *temp = t->root;

  while (temp->right != NULL) {
    temp = temp->right;
  }
  return temp;
}

node_t *max(node_t *cur) {
  node_t *temp = cur;
  while (temp->right != NULL) {
    temp = temp->right;
  }
  return temp;
}

void rbtree_delete_fixup(rbtree *t, node_t *p, node_t *pp) { // NULL 8
  node_t *temp;

  while ((p != t->root) && (!p || p->color == RBTREE_BLACK)) {
    // p가 부모의 왼쪽 자식일 때
    if (p == pp->left) {
      temp = pp->right;
      // 왼쪽 자식 레드 & 형제 레드일때 
      if (temp->color == RBTREE_RED) {
        temp->color = RBTREE_BLACK;
        pp->color = RBTREE_RED;
        leftrotate(pp, t);
        temp = pp->right;
      }
      // 부모, 자식 둘 다 블랙 노드
      if ((!(temp->left) || temp->left->color == RBTREE_BLACK) && (!(temp->right) || temp->right->color == RBTREE_BLACK)) {
        temp->color = RBTREE_RED;
        p = pp;
        pp = pp->parent;
      }
      else {
        if (!(temp->right) || temp->right->color == RBTREE_BLACK) {
          temp->left->color = RBTREE_BLACK;
          temp->color = RBTREE_RED;
          rightrotate(temp, t);
          temp = pp->right;
        }
        temp->color = pp->color;
        pp->color = RBTREE_BLACK;
        if (temp->right) {
          temp->right->color = RBTREE_BLACK;
        }
        leftrotate(pp, t);
        p = t->root;
      }
    }

    else {
      node_t *temp = pp->left;
      if (temp->color == RBTREE_RED) {
        temp->color = RBTREE_BLACK;
        pp->color = RBTREE_RED;
        rightrotate(pp, t);
        temp = pp->left;
      }
      if ((!(temp->right) || temp->right->color == RBTREE_BLACK) && (!(temp->left) || temp->left->color == RBTREE_BLACK)) {
        temp->color = RBTREE_RED;
        p = pp;
        pp = pp->parent;
      }
      else {
        if (!(temp->left) || temp->left->color == RBTREE_BLACK) {
          temp->right->color = RBTREE_BLACK;
          temp->color = RBTREE_RED;
          leftrotate(temp, t);
          temp = pp->left;
        }
        temp->color = pp->color;
        pp->color = RBTREE_BLACK;
        if (temp->left) {
          temp->left->color = RBTREE_BLACK;
        }
        rightrotate(pp, t);
        p = t->root;
      }
    }
  }
  if (p) {
    p->color = RBTREE_BLACK;
  }
}

// target 노드와 그 노드의 parent 노드의 연결관계만을 처리
void rb_transplant(rbtree *t, node_t *u, node_t *v) {  // 2 NULL
  // u가 루트면
  if(u->parent == NULL) {
    t->root = v;
  }
  else {
    // u가 왼쪽 자식이면
    if(u == u->parent->left) {
      u->parent->left = v;
    }
    // u가 오른쪽 자식이면
    else {
      u->parent->right = v;
    }
  }
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase

  node_t *y;
  node_t *x = NULL;

  // 자식 노드가 0 또는 1
  if (!(z->left) || !(z->right)) {
    y = z;
  }
  // 자식 노드가 2
  else {
    y = max(z->left);
  }

  if (y->left) {
    x = y->left;
  } else {
    x = y->right; // 2->right => NULL
  }

  if (x) {
    x->parent = y->parent;
  }
  rb_transplant(t, y, x);
  if (y != z) {
    z->key = y->key;
  }

  if (y->color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x, y->parent); // null
  }
  
  free(y);

  return 0;
}

int inorderArr(node_t *cur, key_t *result, int index) {
  if (cur) {
    index = inorderArr(cur->left, result, index);
    result[index++] = cur->key;
    index = inorderArr(cur->right, result, index);
  }
  return index;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorderArr(t->root, arr, 0);
  return 0;
}
