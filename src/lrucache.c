#include <stdio.h>
#include "lrucache.h"

lrucache_init(LRUCACHE* lrucache, int size)
{
    int err;
    
    lrucache->size = size;
        
    lrucache->map = malloc(sizeof(MAP));
    err = map_init(lrucache->map);
    if (err) {
        return -1;
    }
    
    lrucache->queue = malloc(sizeof(QUEUE));
    err = queue_init(lrucache->queue);
    if (err) {
        return -2;
    }    
    
    return 0;
}

int lrucache_clear(LRUCACHE* lrucache)
{
    int err;
    
    err = map_clear(lrucache->map);
    if (err) {
        return -1;
    }
    
    err = queue_clear(lrucache->queue);
    if (err) {
        return -2;
    }
    
    return 0;
}


char* lrucache_get(LRUCACHE* lrucache, char* key)
{
    queue_remove(lrucache->queue, key);
    queue_insert(lrucache->queue, key);
    
    return map_get(lrucache->map, key);
}

int lrucache_has(LRUCACHE* lrucache, char* key)
{
    return map_has(lrucache->map, key);
}

int lrucache_set(LRUCACHE* lrucache, char* key, char* value)
{
    char* k;
    
    while (map_length(lrucache->map) >= lrucache->size) {
        k = queue_peek(lrucache->queue);
        if (k == NULL) {
            break;
        } else {
            map_remove(lrucache->map, k);
        }
    }
    
    queue_insert(lrucache->queue, key);
    
    return map_set(lrucache->map, key, value);
}

int lrucache_remove(LRUCACHE* lrucache, char* key)
{
    queue_remove(lrucache->queue, key);
    map_remove(lrucache->map, key);
    
    return 0;
}

void lrucache_dispose(LRUCACHE* lrucache)
{
    map_dispose(lrucache->map);
    queue_dispose(lrucache->queue);
}
