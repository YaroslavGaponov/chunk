#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define DEFAULT_MAP_HABLE_SIZE 10
#define MAP_MAX_SORTED_LINKED_LIST_SIZE 5

struct MNODE {
    char* key;
    void* value;
    struct MNODE* next;
};

typedef struct MNODE MNODE;

typedef struct MAP {
    int length;
    int hash_table_size;
    MNODE** htable;
} MAP;

int map_init(MAP* map);
int map_clear(MAP* map);
int map_set(MAP* map, char* key, void* value);
void* map_get(MAP* map, char* key);
int map_remove(MAP* map, char* key);
int map_has(MAP* map, char* key);
void map_dispose(MAP* map);
int map_length(MAP* map);
#endif