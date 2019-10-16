#ifdef EVAL_H
#define EVAL_H

#include "parse.h"

int ast_evalue(sh_ast* ast);
int do_redirect(sh_ast* left, sh_ast* right, int is_left);
int do_and(sh_ast* left, sh_ast* right);
int is(char* s1, char* s2);


#endif
