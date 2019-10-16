#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <assert.h>

#include "eval.h"
#include "parse.h"

// int do_redirect(sh_ast* left, sh_ast* right) {

int do_andor(sh_ast* left, sh_ast* right, int is_and) {
  int cpid;
  if((cpid = fork())) {
    int st;
    waitpid(cpid, &st, 0);
  }
  else {
    int st = ast_evalue(left);
    if(!st ^ is_and) {
      exit(st);
    }
    st = ast_evalue(right);
    exit(st);
  }
  return 0;
}

int do_redirect(sh_ast* left, sh_ast* right, int is_left) {
    int cpid;

    if((cpid = fork())) {
      waitpid(cpid, 0, 0);
    }
    else {
      int pipes[2];
      int rv = pipe(pipes);
      assert(rv != -1);
      int fd;
      if(is_left) {
        fd = open(right->argv[0], O_RDONLY);
        assert(fd != -1);
        close(0);
      }
      else {
        fd = open(right->argv[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
        assert(fd != -1);
        close(1);
      }
      dup(fd);
      close(fd);
      ast_evalue(left);
      exit(EXIT_SUCCESS);
    }
    return 0;
}
int is(char* s1, char* s2) {
  return strcmp(s1, s2) == 0;
}

int ast_evalue(sh_ast* ast) {
    printf("evaluating: %s\n", ast->argv[0]);
    if(ast->op) {
        char* op = ast->op;
        printf("getting op %s\n", op);

        if(is(op, ";")) {
            ast_evalue(ast->left);
            return ast_evalue(ast->right);
        }
        else if(is(op, ">")) {
            do_redirect(ast->left, ast->right, 0);
            return 0;
        }       
        else if(is(op, "<")) {
            do_redirect(ast->left, ast->right, 1);
            return 0;
        }
        else if(is(op, "&&")) {
            printf("doing and, %s, %s\n", ast->left->argv[0], ast->right->argv[0]);
            do_andor(ast->left, ast->right, 1);
            return 0;
        }        
        else if(is(op, "||")) {
            printf("doing or, %s, %s\n", ast->left->argv[0], ast->right->argv[0]);
            do_andor(ast->left, ast->right, 0);
            return 0;
        }
    }

    // built in commands
    if(is(ast->argv[0], "cd")) {
        chdir(ast->argv[1]);
        return 0;
    }
    else if (is(ast->argv[0], "exit")) {
        exit(1);
    }

    int cpid;
    if ((cpid = fork())) {
            // parent process

            int status;
            waitpid(cpid, &status, 0);
            printf("main returning %d after exec\n", status);
            return status;
            // if (WIFEXITED(status)) {
            //     printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            // }
        }
        else {
            // child process
            printf("executing cmd: %s\n", ast->argv[0]);
            printf("executing cmd: %s\n", ast->argv[2]);
            execvp(ast->argv[0], ast->argv);
            printf("Can't get here, exec only returns on error.");
            exit(1);
        }
        return 0;
}