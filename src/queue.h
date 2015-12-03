#ifndef QUEUE_h
#define QUEUE_H

#include <stdlib.h>
#include "map.h"

struct QNODE {
    char *value;
    struct QNODE* prev; 
    struct QNODE* next;    
};

typedef struct QNODE QNODE;

typedef struct QUEUE {
    QNODE* head;
    QNODE* tail;
    MAP* map;
} QUEUE;

int queue_init(QUEUE* queue);
int queue_clear(QUEUE* queue);
int queue_is_empty(QUEUE* queue);
int queue_insert(QUEUE* queue, char* value);
int queue_remove(QUEUE* queue, char* value);
char* queue_peek(QUEUE* queue);
void queue_dispose(QUEUE* queue);

#endif