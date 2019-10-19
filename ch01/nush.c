#include "tokens.h"
#include "list.h"
#include "parse.h"
#include "eval.h"
#include "hashmap.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>


void execute(char* cmd, hashmap* map) {

    list* tokens = tokenize(cmd);
    // print_list(tokens);
    sh_ast* asts = parse(tokens, map);
    ast_evalue(asts);
    free_list(tokens);
    free_ast(asts); 
}

int
main(int argc, char* argv[])
{
    char cmd[100];
    hashmap* map = make_hashmap();

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
            free(du);
        }
        fclose(file);
        cmds = rev_free(cmds);
        list* it = cmds;
        while(it) {
            execute(it->head, map);
            it = it->tail;
        }

        free_list(cmds);
        free_hashmap(map);
        return 0;
    }

    while (1) {
        printf("nush$ ");
        fflush(stdout);
        char* rv = fgets(cmd, 99, stdin);
        if (!rv) {
            exit(0);
        }

        execute(cmd, map);

    }
}
