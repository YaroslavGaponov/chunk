#ifndef LRUCACHE_H
#define LRUCACHE_H

#include "map.h"
#include "queue.h"

typedef struct LRUCACHE {
    int size;
    MAP* map;
    QUEUE* queue;
} LRUCACHE;

int lrucache_init(LRUCACHE* lrucache, int size);
int lrucache_clear(LRUCACHE* lrucache);
char* lrucache_get(LRUCACHE* lrucache, char* key);
int lrucache_remove(LRUCACHE* lrucache, char* key);
int lrucache_has(LRUCACHE* lrucache, char* key);
int lrucache_set(LRUCACHE* lrucache, char* key, char* value);
void lrucache_dispose(LRUCACHE* lrucache);

#endif