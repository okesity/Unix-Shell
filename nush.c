#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokens.h"
#include "list.h"
#include "parse.h"

int ast_evalue(sh_ast* ast) {
    int cpid;
    printf("evaluating: %s\n", ast->argv[0]);
    // char* cmd[] = malloc(sizeof(input_ast->));
    // strncpy(cmd, input_ast->argv, 20);
    if ((cpid = fork())) {
            // parent process

            int status;
            waitpid(cpid, &status, 0);

            printf("== executed program complete ==\n");

            printf("child returned with wait code %d\n", status);
            if (WIFEXITED(status)) {
                printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            }
        }
        else {
            // child process
            printf("Child pid: %d\n", getpid());
            printf("Child knows parent pid: %d\n", getppid());

            printf("== executed program's output: ==\n");
            execvp(ast->argv[0], ast->argv);
            printf("Can't get here, exec only returns on error.");
        }
        return 0;
}


// void
// execute(char* cmd)
// {
//     int cpid;

//     if ((cpid = fork())) {
//         // parent process
//         printf("Parent pid: %d\n", getpid());
//         printf("Parent knows child pid: %d\n", cpid);

//         // Child may still be running until we wait.

//         int status;
//         waitpid(cpid, &status, 0);

//         printf("== executed program complete ==\n");

//         printf("child returned with wait code %d\n", status);
//         if (WIFEXITED(status)) {
//             printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
//         }
//     }
//     else {
//         // child process
//         printf("Child pid: %d\n", getpid());
//         printf("Child knows parent pid: %d\n", getppid());

//         for (int ii = 0; ii < strlen(cmd); ++ii) {
//             if (cmd[ii] == ' ') {
//                 cmd[ii] = 0;
//                 break;
//             }
//         }

//         // The argv array for the child.
//         // Terminated by a null pointer.
//         // char* args[] = {cmd, "one", 0};

//         printf("== executed program's output: ==\n");

//         execvp(cmd[0], cmd);
//         printf("Can't get here, exec only returns on error.");
//     }
// }

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
        print_list(tokens);

        sh_ast* asts = parse(tokens);
        int ret_val = ast_evalue(asts);

        free_list(tokens);
        
    }
}
