#ifndef QUEUE_h
#define QUEUE_H

#include <stdlib.h>


typedef struct QUEUE {
    int length;
} QUEUE;

int queue_init(QUEUE* queue);
int queue_clear();
int queue_insert(QUEUE* queue, char* value);
int queue_remove(QUEUE* queue, char* value);
char* queue_peek(QUEUE* queue);
void queue_dispose(QUEUE* queue);

#endif