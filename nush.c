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
    sh_ast* asts = parse(tokens, map);
    ast_evalue(asts);
    free_list(tokens);
    free_ast(asts); 
}

char* concat(char* s1, char* cmd) {
    char* s2 = strdup(cmd);
    s2[strlen(s2) - 1] = '\0';
    char* res = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(res, s1);
    strcat(res, s2);
    free(s2);
    return res;
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

            //handle new line '\'
            if(du[strlen(du) - 1] == '\\') {
                du[strlen(du) - 1] = '\0';
                fgets(cmd, 100, file);
                char* du2 = concat(du, cmd);
                free(du);
                du = du2;
            }
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
        char* rv = fgets(cmd, 100, stdin);
        if (!rv) {
            exit(0);
        }

        execute(cmd, map);
    }
}
