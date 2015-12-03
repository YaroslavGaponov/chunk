#include <stdio.h>
#include <string.h>
#include "queue.h"

int queue_init(QUEUE* queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    
    return 0;
}

int queue_clear(QUEUE* queue)
{
    int err;
    
    queue_dispose(queue);    
    err = queue_init(queue);
    
    return err;
}

int queue_is_empty(QUEUE* queue) {
    return queue->head == NULL;
}

int queue_insert(QUEUE* queue, char* value)
{
    QNODE* qnode;
    
    qnode = malloc(sizeof(QNODE));
    qnode->value = strdup(value);
    qnode->prev = queue->tail;
    qnode->next = NULL;
    
    if (queue_is_empty(queue)) {
        queue->head = queue->tail = qnode;
    } else {
        queue->tail->next = qnode;
        queue->tail = queue->tail->next;
    }
    
    return 0;
}

int queue_remove(QUEUE* queue, char* value)
{
    QNODE* qnode;
    
    qnode = queue->head;

    while (qnode != NULL) {
        if (strcmp(qnode->value, value) == 0) {
            
            if (qnode->prev != NULL) {
                qnode->prev->next = qnode->next;
            } else {
                queue->head = qnode->next;
            }
            
            if (qnode->next != NULL) {
                qnode->next->prev = qnode->prev;
            } else {
                queue->tail = qnode->prev;
            }
            
            free(qnode->value);
            free(qnode);
            
            return 0;
        }
        qnode = qnode->next;
    }

    return -1;
}

char* queue_peek(QUEUE* queue)
{
    char* value;
    QNODE* qnode;
    
    if (queue->head == NULL) {
        return NULL;
    }
    
    value = queue->head->value;
    qnode = queue->head->next;
    qnode->prev = NULL;
    free(queue->head);
    queue->head = qnode;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    
    return value;
    
}

void queue_dispose(QUEUE* queue)
{
    QNODE* qnode;
    
    qnode = queue->head;
    while (qnode != NULL) {        
        if (qnode->prev != NULL) {
            free(qnode->prev->value);
            free(qnode->prev);
        }
        qnode = qnode->next;
    }
}
