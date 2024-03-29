#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "list.h"
#include "tokens.h"


char* slice_string(const char* text, int start, int end) {
   int nn = end - start + 2;
   char* res = malloc(nn);
   memcpy(res, text + start, nn);
   // printf("Sliding %s, %d, %d\n", res, start, end);
   res[nn - 1] = '\0';
   if (res[nn - 2] == '\n')
      res[nn - 2] = '\0';
   return res;
}

int isoperator(const char text) {
   const char ops[] = {'>', '<', '|', '&', ';'};
   for(int i = 0; i < 6; ++i) {
      if(text == ops[i])
         return 1;
   }
   return 0;
}
int isparen(const char c) {
   return (c=='(' || c==')');
}

// adding [start, end] from text to head of xs, return new head
list* add_cons(const char* text, list* xs, int start, int end) {
   if (start > end)
      return xs;
   char* command = slice_string(text, start, end);
   xs = cons(command, xs);
   free(command);
   return xs;
}

int find_alignparen(char* str, int start) {
   int cnt = 1; 
   int end = strlen(str);
   for(int i=start; i<=end; i++) {
      if(str[i] == '\"') {
         do {
            i++;
         }
         while(str[i] != '\"');

      }
      if(str[i] == '(') {
         cnt++;
      }
      else if(str[i] == ')') {
         cnt--;
      }

      if(cnt == 0) {
         return i;
      }
  }
  puts("parens do not align!");
  return -1;
}

list* tokenize(char* text) {
   list* xs = 0;
   int ii = 0;
   int len = strlen(text);
   while(ii < len) {
      if(isspace(text[ii])) {
         ii ++;
      }
      // else if (text[ii] == '\\') {
      //    char* rv = fgets(text, 99, stdin);
      //    if (!rv) {
      //       exit(0);
      //    }
      //    ii = 0;
      // }
      else if (text[ii] == '\"') {
         int nn = ii + 1;
         while(text[nn] != '\"') {
            nn++;
         }
         xs = add_cons(text, xs, ii + 1, nn - 1);
         ii = nn + 1;
      }
      else if (isoperator(text[ii])) {
         int nn = ii;
         while(isoperator(text[nn + 1])) {
            nn++;
         }
         xs = add_cons(text, xs, ii, nn);
         ii = nn + 1;
      }
      else if (text[ii] == '(') {
         int index = find_alignparen(text, ii + 1);
         if(index != -1) {
            xs = add_cons(text, xs, ii, index);
            ii = index + 1;
         }
         else {
            ii++;
         }
      }
      else {
         int nn = ii;
         while(!isspace(text[nn + 1]) && !isoperator(text[nn + 1]) && !isparen(text[nn + 1])) {
            nn++;
         }
         xs = add_cons(text, xs, ii, nn);
         ii = nn + 1;
      }
   }

   return xs;
}

