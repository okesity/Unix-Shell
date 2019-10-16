#include "tokens.h"
#include "list.h"
#include "parse.h"
#include "eval.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


int
main(int argc, char* argv[])
{
    char cmd[100];

    while (1) {
        printf("nush$ ");
        fflush(stdout);
        char* rv = fgets(cmd, 99, stdin);
        if (!rv) {
            exit(0);
        }
        list* tokens = tokenize(cmd);

        sh_ast* asts = parse(tokens);
        int ret_val = ast_evalue(asts);
        free_list(tokens);
        
    }
}
