
#include "chunk.h"

int chunk_create(char* filename, int hash_table_size)
{
    return driver_create(filename, hash_table_size);
}

int chunk_open(CHUNK* chunk, char* filename, int cache_size)
{
    int err;
    
    chunk->driver = malloc(sizeof(DRIVER));
    err = driver_open(chunk->driver, filename);
    if (err) {        
        return -1;
    }
    
    chunk->lrucache = malloc(sizeof(LRUCACHE));
    err = lrucache_init(chunk->lrucache, cache_size);
    if (err) {
        return -2;
    }
    
    return 0;
}

void chunk_set(CHUNK* chunk, char* key, char* value)
{
    lrucache_set(chunk->lrucache, key, value);
    driver_set(chunk->driver, key, value);
}

char* chunk_get(CHUNK* chunk, char* key)
{
    if (lrucache_has(chunk->lrucache, key)) {
        return lrucache_get(chunk->lrucache, key);
    }
    return driver_get(chunk->driver, key);
}

int chunk_has(CHUNK* chunk, char* key)
{
    if (lrucache_has(chunk->lrucache, key)) {
        return 1;
    }
    return driver_has(chunk->driver, key);    
}

int chunk_remove(CHUNK* chunk, char* key)
{
    lrucache_remove(chunk->lrucache, key);
    driver_remove(chunk->driver, key);    
}

void chunk_close(CHUNK* chunk)
{
    lrucache_dispose(chunk->lrucache);
    driver_close(chunk->driver);
}

void chunk_for_each(CHUNK* chunk, int (*iterator)(char* key, char* value))
{
    driver_for_each(chunk->driver, iterator);
}
