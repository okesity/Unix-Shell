#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

char* operators[] = {"<", ">", "&&", "||", "&", "|", ";"};

void print_ast(sh_ast* ast) {
  if(!ast) {
    puts("NULL");
    return;
  }
  puts("=========");
  printf("op: %s\n", ast->op);
  for(int i=0;i<4;i++) {
    printf("%s\t", ast->argv[i]);
  }
  puts("=========");
  puts("left:");
  print_ast(ast->left);
  puts("right:");
  print_ast(ast->right);
}

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

void
free_ast(sh_ast* ast) {
  for(int i=0;i<20;i++) {
    free(ast->argv[i]);
  }
  free(ast);
}

// attribution: function slice taken from classnotes
list*
rev_slice(list *xs, int i0, int i1) {
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

//return new list sliced from xs
list*
slice(list* xs, int i0, int i1)
{
  list* ys = rev_slice(xs, i0, i1); 
  return rev_free(ys);
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
  free_list(toks);
  return res;
}

int is_op(char* str) {
  for(int i=0; i<7;i++) {
    if(strcmp(str, operators[i]) == 0)
      return 1;
  }
  return 0;
}

sh_ast* parse(list* toks) {
  if(!toks) {
    return NULL;
  }
  int len = 0;
  list* it = toks;
  while(it) {
    if(is_op(it->head)) {
      char* op = strdup(it->head);
      // printf("found op: %s\n", op);
      list* part = slice(toks, 0, len);
      return make_ast(op, parse(it->tail), parse(part), NULL, 0);
    }
    it = it->tail;
    len++;
  }
  
  list* part = rev_slice(toks, 0, len);
  char** com = to_array(part, len);

  return make_ast(NULL, NULL, NULL, com, len + 1);
}