#include <stdio.h>
#include <string.h>
#include "queue.h"

int queue_init(QUEUE* queue)
{   
    queue->length = 0;
    
    return 0;
}

int queue_clear(QUEUE* queue)
{
    
    return 0;
}

int queue_insert(QUEUE* queue, char* value)
{
    
    return 0;
}

int queue_remove(QUEUE* queue, char* value)
{
   
   return 1; 
}

char* queue_peek(QUEUE* queue)
{
    return NULL;
}

void queue_dispose(QUEUE* queue)
{
}
