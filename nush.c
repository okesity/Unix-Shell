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
    // printf("getting cmd:%s\n", cmd);
    list* tokens = tokenize(cmd);
    sh_ast* asts = parse(tokens);
    int ret_val = ast_evalue(asts);
}

int
main(int argc, char* argv[])
{
    char cmd[100];
    if(argc == 2) {
        list* cmds = NULL;
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            puts("input file error!");
            exit(1);
        }
        //add all commands to list
        while(fgets(cmd, 100, file)) {
            char* du = strdup(cmd);
            du[strlen(du) - 1] = '\0';
            cmds = cons(du, cmds);
        }
        cmds = reverse(cmds);
        list* it = cmds;
        while(it) {
            execute(it->head);
            it = it->tail;
        }
        free_list(cmds);
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
