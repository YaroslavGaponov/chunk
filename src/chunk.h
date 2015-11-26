#ifndef CHUNK_H
#define CHUNK_H

#include "driver.h"
#include "lrucache.h"

typedef struct CHUNK {
    DRIVER* driver;
    LRUCACHE* lrucache;
} CHUNK;

int chunk_create(char* filename, int hash_table_size);
int chunk_open(CHUNK* chunk, char* filename, int cache_size);
void chunk_set(CHUNK* chunk, char* key, char* value);
char* chunk_get(CHUNK* chunk, char* key);
int chunk_has(CHUNK* chunk, char* key);
int chunk_remove(CHUNK* chunk, char* key);
void chunk_close(CHUNK* chunk);
void chunk_for_each(CHUNK* chunk, int (*iterator)(char* key, char* value));

#endif