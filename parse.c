#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

sh_ast* 
make_ast(char* op, sh_ast* left, sh_ast* right, char** argv, int len) {
  // for(int i=0;i<len;i++)
    // printf("making ast: %s\n", argv[i]);
  sh_ast* res = malloc(sizeof(sh_ast));
  res->op = op;
  res->left = left;
  res->right = right;
  for(int i=0;i<len;i++) {
    res->argv[i] = argv[i]; 
  }
  return res;
}

// attribution: func slice taken from classnotes
list*
rev_slice(list* xs, int i0, int i1)
{
  list* ys = 0;
  list* it = xs;
  for (int ii = 0; ii < i0; ++ii) {
    it = it->tail;
  }
  for (int ii = i0; ii < i1; ++ii) {
    ys = cons(it->head, ys);
    it = it->tail;
  }
  return ys;
}

char** to_array(list* toks, int count) {
  char** res = malloc((count + 1) * sizeof(char*));
  list* it = toks;
  for(int i=0; i<count;i++) {
    res[i] = strdup(it->head);
    // printf("copying: %s\n", res[i]);
    it = it->tail;
  }
  res[count] = 0;
  free(it);
  return res;
}

sh_ast* parse(list* toks) {
  int len = 0;
  list* it = toks;
  while(it) {
    it = it->tail;
    len++;
  }
  list* part = rev_slice(toks, 0, len);
  // printf("\nmaking ast from: %d\n", len);
  char** com = to_array(part, len);
  // for(int i=0;i<len;i++) {
    // printf("array: %s", com[i]);
  // }
  return make_ast("=", NULL, NULL, com, len + 1);
}