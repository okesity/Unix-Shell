
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install libbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    // TODO: Produce an appropriate hash value.
    int res = 0;
    for (int i=0;key[i] != '\0'; i++) {
        res += 67*(int) key[i];
    }
    return res;
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = malloc(sizeof(hashmap));
    hh->size = 0;
    hh->capacity = nn;
    hh->data = calloc(nn, sizeof(hashmap_pair));
    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    free(hh);
}

char*
hashmap_get(hashmap* hh, char* kk)
{
    int hash_val = hash(kk) % hh->capacity;

    hashmap_pair cur_pair = hh->data[hash_val];
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0) {
            return strdup(cur_pair.val);
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hh->data[hash_val];
    }
    return NULL;
}

void
hashmap_put(hashmap* hh, char* kk, char* vv)
{
    if (hh->size + 1 >= hh->capacity) {
        hashmap* new_map = make_hashmap_presize(2 * hh->capacity);
        for(int i=0;i<hh->capacity;i++) {
            hashmap_pair pair = hashmap_get_pair(hh, i);
            if(pair.used && !pair.tomb) {
                hashmap_put(new_map, pair.key, pair.val);
            }
        }
        hashmap_pair* data = new_map->data;       
        free(new_map);
        free(hh->data);
        hh->capacity *= 2;
        hh->data = data;
    }

    int hash_val = hash(kk) % hh->capacity;

    hashmap_pair cur_pair = hashmap_get_pair(hh, hash_val);
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0){
            strcpy(hh->data[hash_val].val, vv);
            return;
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hashmap_get_pair(hh, hash_val);
    }

    hh->data[hash_val].used = true;
    strcpy(hh->data[hash_val].val, vv);
    strcpy(hh->data[hash_val].key, kk);

    hh->size++;
}

void
hashmap_del(hashmap* hh, char* kk)
{
    int hash_val = hash(kk) % hh->capacity;

    hashmap_pair cur_pair = hashmap_get_pair(hh, hash_val);
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0){
            cur_pair.tomb = true;
            return;
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hashmap_get_pair(hh, hash_val);
    }
}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    return hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    for(int i=0;i<hh->capacity; i++) {
        hashmap_pair cur = hashmap_get_pair(hh, i);
        printf("{%s, %s, tomb: %d, used: %d}\n", cur.key, cur.val, cur.tomb, cur.used);
        // printf("{%s, %d}\n", cur->key, cur->val);
    }
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
}
