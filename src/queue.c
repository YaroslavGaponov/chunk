
#include <string.h>
#include "queue.h"

int queue_init(QUEUE* queue)
{   
    queue->root = NULL;
    queue->length = 0;
    
    return 0;
}

int queue_clear(QUEUE* queue)
{
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->root;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        free(pred);
    }

    queue->root = NULL;
    queue->length = 0;
    
    return 0;
}

int queue_insert(QUEUE* queue, char* value)
{
    QNODE* node;
    
    node = malloc(sizeof(QNODE));
    node->value = strdup(value);
    node->next = queue->root;
    queue->root = node;
    
    queue->length++;
    
    return 0;
}

int queue_remove(QUEUE* queue, char* value)
{
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->root;
    pred = NULL;
    while (curr != NULL) {
        if (strcmp(curr->value, value) == 0) {
            if (pred == NULL) {                
                queue->root = curr->next;                
            } else {
                pred->next = curr->next;
            }
            free(curr->value);
            free(curr);
            queue->length--;
            return 0;
        }
        pred = curr;
        curr = curr->next;
    }
   
   return 1; 
}

char* queue_peek(QUEUE* queue)
{
    QNODE* node;
    char* value;
    
    node = queue->root;
    if (node != NULL) {
        value = node->value;
        queue->root = node->next;
        free(node->value);
        free(node);
        queue->length--;
        return value;
    }
    return NULL;
}

void queue_dispose(QUEUE* queue)
{
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->root;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        free(pred->value);
        free(pred);
    }
}
