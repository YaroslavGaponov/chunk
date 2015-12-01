
#include <string.h>

#include "map.h"
#include "hash.h"

static int map__init(MAP* map, int hash_table_size)
{
    map->hash_table_size = hash_table_size;
    map->htable = malloc(sizeof(struct MNODE *) * map->hash_table_size);
    map->length = 0;

    return 0;
}

int map_init(MAP* map) {
    return map__init(map, DEFAULT_MAP_HABLE_SIZE);
}

int map_clear(MAP* map)
{
    MNODE* curr;
    MNODE* prev;
    int i;
    
    for(i = 0; i < map->hash_table_size; i++) {
        curr = map->htable[i];
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            
            free(prev->key);
            free(prev);
        }
        map->htable[i] = NULL;
    }
   
    map->length = 0;
    
    return 0;
}

int map_set(MAP* map, char* key, void* value)
{
    MAP* _map;
    MNODE* curr;
    MNODE* prev;
    MNODE* node;
    int cmp;
    int indx;
    int i;
    
    node = malloc(sizeof(MNODE));    
    node->key = strdup(key);
    node->value = value;
    node->next = NULL;
    
    indx = hash_get(key) % map->hash_table_size;
    
    curr = map->htable[indx];
    prev = NULL;
    while(curr != NULL) {
        cmp = strcmp(curr->key, key);
        if (cmp == 0) {
            if (prev !=  NULL) {
                prev->next = node;
            } else {
                map->htable[indx] = node;
            }
            node->next = curr->next;
            free(curr->key);
            free(curr);            
            goto done;
        }
        if (cmp > 0) {
            if (prev != NULL) {
                prev->next = node;
            } else {
                map->htable[indx] = node;
            }
            node->next = curr;
            map->length++;            
            goto done;
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    if (prev != NULL) {
        prev->next = node;
    } else {
        map->htable[indx] = node;
    }
    map->length++;
    
    done:
    if ((map->length / map->hash_table_size) > MAP_MAX_SORTED_LINKED_LIST_SIZE) {
        _map = malloc(sizeof(MAP));
        map__init(_map, map->hash_table_size << 1);
        for(i = 0; i < map->hash_table_size; i++) {
            curr = map->htable[i];
            while (curr != NULL) {
                map_set(_map, curr->key, curr->value);
            }
        }
        map_dispose(map);
        map = _map;
    }    
    return 0;
}

void* map_get(MAP* map, char* key)
{
    MNODE* curr;
    int cmp;
    int indx;
    
    indx = hash_get(key) % map->hash_table_size;
    curr = map->htable[indx];
    
    while (curr != NULL) {
        cmp = strcmp(curr->key, key);        
        if (cmp == 0) {            
            return curr->value;
        }
        if (cmp > 0) {
            return NULL;
        }
        curr = curr->next;
    }
    return NULL;
}

int map_has(MAP* map, char* key)
{
    MNODE* curr;
    int cmp;
    int indx;
    
    indx = hash_get(key) % map->hash_table_size;
    curr = map->htable[indx];
    
    while (curr != NULL) {
        cmp = strcmp(curr->key, key);        
        if (cmp == 0) {
            return 1;
        }
        if (cmp > 0) {
            return 0;
        }
        curr = curr->next;
    }
    return 0;    
}

int map_remove(MAP* map, char* key)
{
    MNODE* curr;
    MNODE* prev;
    int cmp;
    int indx;
    
    indx = hash_get(key) % map->hash_table_size;
    curr = map->htable[indx];
    prev = NULL;
    while (curr != NULL) {
        cmp = strcmp(curr->key, key);        
        if (cmp == 0) {
            if (prev == NULL) {
                map->htable[indx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            free(curr->key);
            free(curr);
            
            map->length--;
            
            return 0;
        }
        if (cmp > 0) {
            return -1;
        }
        
        prev = curr;
        curr = curr->next;
    }
    return -2;
}

int map_length(MAP* map)
{
    return map->length;
}

void map_dispose(MAP* map)
{
    MNODE* curr;
    MNODE* prev;
    int i;
    
    for(i = 0; i < map->hash_table_size; i++) {
        curr = map->htable[i];
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            
            free(prev->key);
            free(prev);
        }
    }
    free(map->htable);
}
