#ifndef EVAL_H
#define EVAL_H

#include "parse.h"

int ast_evalue(sh_ast* ast);
int do_pipe(sh_ast* left, sh_ast* right);
int do_background(sh_ast* ast);
int do_andor(sh_ast* left, sh_ast* right, int is_and);
int do_redirect(sh_ast* left, sh_ast* right, int is_left);
int do_andOr(sh_ast* left, sh_ast* right, int is_and);
int is(char* s1, char* s2);


#endif
