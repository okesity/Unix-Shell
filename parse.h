#ifndef PARSE_H
#define PARSE_H

// #include "ast.h"
#include "list.h"
#include "hashmap.h"

typedef struct sh_ast {
  char* op;
  struct sh_ast* left;
  struct sh_ast* right;
  char* argv[20];
} sh_ast;

typedef struct dict {
  char key[10];
  char val[10];
} dict;

sh_ast* make_ast(char* op, sh_ast* left, sh_ast* right, char** argv, int len);
void free_ast(sh_ast* ast);

void print_ast(sh_ast* ast);

list* slice(list* xs, int i0, int i1);
list* rev_slice(list* xs, int i0, int i1);
char** to_array(list* toks, int count);
sh_ast* parse(list* toks, hashmap* map);

#endif