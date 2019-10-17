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
#include <signal.h>

void execute(char* cmd) {
    list* tokens = tokenize(cmd);
    sh_ast* asts = parse(tokens);
    // print_ast(asts);
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
        fclose(file);

        cmds = reverse(cmds);
        list* it = cmds;
        int cnt = 0;
        int pid = getpid();
        while(it) {
            // printf("iterating %s\n", it->head);
            // if(getpid() != pid) {
            //     exit(0);
            // }
            execute(it->head);
            it = it->tail;
            cnt++;
        }
        // printf("pid: %d\n", getpid());

        free_list(cmds);
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
