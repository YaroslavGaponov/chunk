#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

struct MNODE {
    char* key;
    char* value;
    struct MNODE* next;
};

typedef struct MNODE MNODE;

typedef struct MAP {
    int length;
    MNODE* root;
} MAP;

int map_init(MAP* map);
int map_clear(MAP* map);
int map_set(MAP* map, char* key, char* value);
char* map_get(MAP* map, char* key);
int map_remove(MAP* map, char* key);
int map_has(MAP* map, char* key);
void map_dispose(MAP* map);
int map_length(MAP* map);

#endif