#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "hashmap.h"

char* operators[] = {"<", ">", "&&", "||", "&", "|", ";"};


void print_ast(sh_ast* ast) {
  if(!ast) {
    puts("NULL");
    return;
  }
  puts("=========");
  printf("op: %s\n", ast->op);
  for(int i=0;i<4;i++) {
    printf("%s\t", ast->argv[i]);
  }
  puts("=========");
  puts("left:");
  print_ast(ast->left);
  puts("right:");
  print_ast(ast->right);
}

sh_ast* 
make_ast(char* op, sh_ast* left, sh_ast* right, char** argv, int len) {
  // for(int i=0;i<len;i++)
    // printf("making ast: %s\n", argv[i]);
  sh_ast* res = calloc(1, sizeof(sh_ast));
  res->op = op;
  res->left = left;
  res->right = right;
  for(int i=0;i<len;i++) {
    res->argv[i] = argv[i];
    // free(argv[i]);
  }
  free(argv);
  // for(int i=0;i<len;i++)
  //   printf("adding : %s\n", res->argv[i]);
  return res;
}

void
free_ast(sh_ast* ast) {
  if(!ast) {
    return;
  }
  for(int i=0;i<20;i++) {
    free(ast->argv[i]);
  }
  free(ast->op);
  free_ast(ast->left);
  free_ast(ast->right);
  free(ast);
}

// attribution: function slice taken from classnotes
list*
rev_slice(list *xs, int i0, int i1) {
  list* ys = 0;
  list* it = xs;
  for (int ii = 0; ii < i0; ++ii) {
    it = it->tail;
  }
  for (int ii = i0; ii < i1; ++ii) {
    ys = cons(it->head, ys);
    it = it->tail;
  }
  return ys;
}

//return new list sliced from xs
list*
slice(list* xs, int i0, int i1)
{
  list* ys = rev_slice(xs, i0, i1); 
  return rev_free(ys);
}

char** 
to_array(list* toks, int count) {
  char** res = malloc((count + 1) * sizeof(char*));
  list* it = toks;
  for(int i=0; i<count;i++) {
    res[i] = strdup(it->head);
    // printf("copying: %s\n", res[i]);
    it = it->tail;
  }
  res[count] = 0;
  return res;
}

int is_op(char* str) {
  for(int i=0; i<7;i++) {
    if(strcmp(str, operators[i]) == 0)
      return 1;
  }
  return 0;
}

int found_assign(char* str) {
  for(int i=0;i<strlen(str);i++) {
    if(str[i] == '=') {
      return i;
    }
  }
  return -1;
}

// char* get_dict(char* str, char** dict) {
//   char* var = str + 1;
//   printf("finding %s\n", var);
//   for(int i=0;i<dic_it;i++) {
//     printf("searching %s\n", dict[i]);
//     if(strcmp(var, dict[i]) == 0) {
//       return strdup(dict[i+1]);
//     }
//   }
//   puts("cannot found from dict");
//   return 0;
// }

sh_ast* parse(list* toks, hashmap* map) {
  if(!toks) {
    return NULL;
  }
  int len = 0;
  list* it = toks;
  static dict* dicts[10];
  
  while(it) {
    //check
    if(found_assign(it->head) != -1) {
      char* line = it->head;
      int len1 = found_assign(line);
      int len2 = strlen(line) - len1 - 1;
      char left[10];
      char right[10];
      memcpy(left, line, len1);
      left[len1] = '\0';
      memcpy(right, &line[len1 + 1], len2);
      right[len2] = '\0';

      hashmap_put(map, left, right);
      return NULL;
      // printf("dict: %s %s", left, hashmap_get(map, left));
    }
    else if(it->head[0] == '$') {
      char* key = it->head + 1;
      char* val = hashmap_get(map, key);
      // printf("%s found value %s\n", key, val);
      if(val) {
        free(it->head);
        it->head = val;
      }
    }
    else if(is_op(it->head)) {
      char* op = strdup(it->head);
      // printf("found op: %s\n", op);
      list* part = slice(toks, 0, len);
      sh_ast* res = make_ast(op, parse(it->tail, map), parse(part, map), NULL, 0);
      // free_list(part);
      return res;
    }
    it = it->tail;
    len++;
  }
  
  list* part = rev_slice(toks, 0, len);
  char** com = to_array(part, len);
  free_list(part);

  return make_ast(NULL, NULL, NULL, com, len + 1);
}