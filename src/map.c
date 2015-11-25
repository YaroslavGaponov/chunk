
#include <string.h>

#include "map.h"

int map_init(MAP *map)
{
    map->root = NULL;
    map->length = 0;

    return 0;
}

int map_clear(MAP *map)
{
    MNODE *curr, *pred;
    
    curr = map->root;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        
        free(pred->key);
        free(pred->value);
        free(pred);
    }
    
    map->root = NULL;
    map->length = 0;
    
    return 0;
}

int map_set(MAP *map, char *key, char *value)
{
    MNODE *curr, *pred, *node;
    int cmp;
    
    node = malloc(sizeof(MNODE));    
    node->key = strdup(key);
    node->value = strdup(value);
    node->next = NULL;
    
    curr = map->root;
    pred = NULL;
    while(curr != NULL) {
        cmp = strcmp(curr->key, key);
        if (cmp == 0) {
            if (pred !=  NULL) {
                pred->next = node;
            } else {
                map->root = node;
            }
            node->next = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);            
            return 0;
        }
        if (cmp > 0) {
            if (pred != NULL) {
                pred->next = node;
            } else {
                map->root = node;
            }
            node->next = curr;
            map->length++;            
            return 0;
        }
        
        pred = curr;
        curr = curr->next;
    }
    
    if (pred != NULL) {
        pred->next = node;
    } else {
        map->root = node;
    }
    map->length++;
    
    return 0;
}

char *map_get(MAP *map, char *key)
{
    MNODE *curr;
    int cmp;
    
    curr = map->root;
    
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

int map_has(MAP *map, char *key)
{
    MNODE *curr;
    int cmp;
    
    curr = map->root;
    
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

int map_remove(MAP *map, char *key)
{
    MNODE *curr, *pred;
    int cmp;
    
    curr = map->root;
    pred = NULL;
    while (curr != NULL) {
        cmp = strcmp(curr->key, key);        
        if (cmp == 0) {
            pred->next = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            map->length--;
            return 1;
        }
        if (cmp > 0) {
            return 0;
        }
        
        pred = curr;
        curr = curr->next;
    }
    return 0;
}

int map_length(MAP *map)
{
    return map->length;
}

void map_dispose(MAP *map)
{
    MNODE *curr, *pred;
    
    curr = map->root;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        
        free(pred->key);
        free(pred->value);
        free(pred);
    }
}
