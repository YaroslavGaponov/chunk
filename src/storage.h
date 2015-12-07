#ifndef STORAGE_H
#define STORAGE_H

#include "chunk.h"

typedef struct STORAGE {
    CHUNK* master;
    int length;
    CHUNK** chunks;
} STORAGE;

int storage_create(char* path, int chunks, int hash_table_size);
int storage_open(STORAGE* storage, char* path, int cache_size);
void storage_set(STORAGE* storage, char* key, char* value);
char* storage_get(STORAGE* storage, char* key);
int storage_has(STORAGE* storage, char* key);
int storage_remove(STORAGE* storage, char* key);
void storage_close(STORAGE* storage);
void storage_for_each(STORAGE* storage, int (*iterator)(char* key, char* value));

#endif