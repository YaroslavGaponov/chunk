
#include <limits.h>
#include "storage.h"

int storage_create(char* path, int chunks, int hash_table_size)
{
    int i;
    char filename[PATH_MAX];
    char tmp[255];
    CHUNK* master;    

    sprintf(filename, "%s/master", path);
    chunk_create(filename, hash_table_size);
    
    master = malloc(sizeof(CHUNK));    
    chunk_open(master, filename, 5);
    
    sprintf(tmp, "%d", chunks);    
    chunk_set(master, "chunks", tmp);

    for(i = 0; i < chunks; i++) {
        sprintf(filename, "%s/chunk_%d", path, i);
        chunk_create(filename, hash_table_size);
    }
    
    chunk_close(master);
    free(master);
    
    return 0;
}

int storage_open(STORAGE* storage, char* path, int cache_size)
{
    char filename[PATH_MAX];
    int i;
    int err;
    
    storage->master = malloc(sizeof(CHUNK));    
    sprintf(filename, "%s/master", path);    
    err = chunk_open(storage->master, filename, 100);
    storage->length = atoi(chunk_get(storage->master, "chunks"));    
    storage->chunks = malloc(sizeof(CHUNK) * storage->length);

    for(i = 0; i < storage->length; i++) {
        sprintf(filename, "%s/chunk_%d", path, i);
        storage->chunks[i] = malloc(sizeof(CHUNK));
        chunk_open(storage->chunks[i], filename, cache_size);
    }

    return 0;    
}

void storage_set(STORAGE* storage, char* key, char* value)
{
    int indx;
    
    indx = hash_get(key) % storage->length;
    
    chunk_set(storage->chunks[indx], key, value);
}

char* storage_get(STORAGE* storage, char* key)
{
    int indx;
    
    indx = hash_get(key) % storage->length;
    
    return chunk_get(storage->chunks[indx], key);
    
}

int storage_has(STORAGE* storage, char* key)
{
    int indx;
    
    indx = hash_get(key) % storage->length;
    
    return chunk_has(storage->chunks[indx], key);    
}

int storage_remove(STORAGE* storage, char* key)
{
    int indx;
    
    indx = hash_get(key) % storage->length;
    
    return chunk_remove(storage->chunks[indx], key);    
}

void storage_close(STORAGE* storage)
{
    int i;
    
    for(i = 0; i < storage->length; i++) {
        chunk_close(storage->chunks[i]);
        free(storage->chunks[i]);
    }
    
    chunk_close(storage->master);
    
    free(storage->master);
}

void storage_for_each(STORAGE* storage, int (*iterator)(char* key, char* value))
{
    int i;
    
    for(i = 0; i < storage->length; i++) {
        chunk_for_each(storage->chunks[i], iterator);
    }    
}
