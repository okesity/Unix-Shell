#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

typedef struct hashmap_pair {
    char key[10]; // null terminated strings
    char val[10];
    bool used;
    bool tomb;
} hashmap_pair;

typedef struct hashmap {
    long useless_field;
    long size;
    long capacity;
    hashmap_pair* data;
    // TODO: Define working hashmap struct.
} hashmap;

hashmap* make_hashmap();
void free_hashmap(hashmap* hh);
int hashmap_has(hashmap* hh, char* kk);
char* hashmap_get(hashmap* hh, char* kk);
void hashmap_put(hashmap* hh, char* kk, char* vv);
void hashmap_del(hashmap* hh, char* kk);
hashmap_pair hashmap_get_pair(hashmap* hh, int ii);
void hashmap_dump(hashmap* hh);

#endif
