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
#include "tokens.h"
#include "hashmap.h"

int do_pipe(sh_ast* left, sh_ast* right) {
  int cpid;
  if((cpid = fork())) {
    waitpid(cpid, 0, 0);
  }
  else {
    int rv;
    int pipes[2];
    rv = pipe(pipes);
    assert(rv != -1);

    if((cpid = fork())) {
      close(0);
      close(pipes[1]);
      dup(pipes[0]);
      waitpid(cpid, 0, 0);
    }
    else {
      close(1);
      close(pipes[0]);
      dup(pipes[1]);
      rv = ast_evalue(left);
      exit(rv);
    }
    rv = ast_evalue(right);
    exit(rv);
  }
  return 0;
}

int do_background(sh_ast* cmd) {
  int cpid;
  if(!(cpid = fork())) {
    int st = ast_evalue(cmd);
    exit(st);
  }
  return 0;
}

int do_andOr(sh_ast* left, sh_ast* right, int is_and) {
  int cpid;

  if((cpid = fork())) {
    int rv; 
    int st;
    rv = waitpid(cpid, &st, 0);
    assert(rv != -1);
    if(WEXITSTATUS(st)) {
      exit(EXIT_FAILURE);
    }
    return rv;

  }
  else {
    int st = ast_evalue(left);
    if(!st ^ is_and) {
      exit(st);
    }
    st = ast_evalue(right);
    return st;   
  }
  return 0;
}

int do_redirect(sh_ast* left, sh_ast* right, int is_left) {
    int cpid;
    // printf("redirect to: %s\n", right->argv[0]);
    
    if((cpid = fork())) {
      waitpid(cpid, 0, 0);
    }
    else {
      int fd;
      if(is_left) {
        fd = open(right->argv[0], O_RDONLY);
        assert(fd != -1);
        close(0);
      }
      else {
        fd = open(right->argv[0], O_CREAT | O_WRONLY, 0644);
        assert(fd != -1);
        close(1);
      }
      dup(fd);
      close(fd);
      int rv = ast_evalue(left);
      exit(rv);
    }
    return 0;
}
int is(char* s1, char* s2) {
  return strcmp(s1, s2) == 0;
}

int ast_evalue(sh_ast* ast) {
    if(!ast) {
      return 0;
    }
    if(ast->op) {
        char* op = ast->op;

        if(is(op, ";")) {
            int rv;
            ast_evalue(ast->left);
            rv = ast_evalue(ast->right);
            return rv;
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
            do_andOr(ast->left, ast->right, 1);
            return 0;
        }        
        else if(is(op, "||")) {
            do_andOr(ast->left, ast->right, 0);
            return 0;
        }
        else if(is(op, "&")) {
            do_background(ast->left);
            return 0;
        }
        else if(is(op, "|")) {
            do_pipe(ast->left, ast->right);
            return 0;
        }
    }

    // built in commands
    if(is(ast->argv[0], "cd")) {
        chdir(ast->argv[1]);
        return 0;
    }
    else if (is(ast->argv[0], "exit")) {
        exit(EXIT_FAILURE);
    }

    int cpid;
    if ((cpid = fork())) {
        // parent process

        int status;
        int rv;
        rv = waitpid(cpid, &status, 0);
        assert(rv != -1);

        return status;
    }
    else {
        if (ast->argv[0] && ast->argv[0][0] == '(') {
          char* cmd = strdup(ast->argv[0] + 1);
          cmd[strlen(cmd) - 1] = '\0';
          list* tokens = tokenize(cmd);
          hashmap* map = make_hashmap();
          sh_ast* asts = parse(tokens, map);
          ast_evalue(asts);
          free_list(tokens);
          free_ast(asts);

          exit(1);
        }
        execvp(ast->argv[0], ast->argv);
        printf("Can't get here, exec only returns on error.");
        exit(1);
    }

    return 0;
}