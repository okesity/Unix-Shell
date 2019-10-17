#include "tokens.h"
#include "list.h"
#include "parse.h"
#include "eval.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

void execute(char* cmd) {
    list* tokens = tokenize(cmd);
    sh_ast* asts = parse(tokens);
    int ret_val = ast_evalue(asts);
    free_list(tokens);
}

int
main(int argc, char* argv[])
{
    char cmd[100];
    
    if(argc == 2) {
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            puts("input file error!");
            exit(1);
        }
        while(fgets(cmd, 99, file)) {
            printf("getting command: %s", cmd);
            execute(cmd);
        }
        fclose(file);
        return 0;
    }

    while (1) {
        printf("nush$ ");
        fflush(stdout);
        char* rv = fgets(cmd, 99, stdin);
        if (!rv) {
            exit(0);
        }

        execute(cmd);
        
    }
}
