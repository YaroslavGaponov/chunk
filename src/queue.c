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
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->head;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        free(pred->value);
        free(pred);
    }
    
    queue->head = NULL;
    queue->tail = NULL;

    return 0;
}

int queue_is_empty(QUEUE* queue) {
    return queue->head == NULL;
}

int queue_insert(QUEUE* queue, char* value)
{
    QNODE* node;
    
    node = malloc(sizeof(QNODE));
    node->value = strdup(value);
    node->next = NULL;
    
    if (queue->head == NULL) {
        queue->head = queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = queue->tail->next;
    }
    
    return 0;
}

int queue_remove(QUEUE* queue, char* value)
{
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->head;
    pred = NULL;    
    while (curr != NULL) {
        if (strcmp(curr->value, value) == 0) {
            if (queue->tail == curr) {
                if (curr->next == NULL) {
                    queue->tail = pred;
                } else {
                    queue->tail = curr->next;
                }
            }
            if (pred == NULL) {
                queue->head = curr->next;
            } else {
                pred->next = curr->next;
            }
            free(curr->value);
            free(curr);
            return 0;
        }
        pred = curr;
        curr = curr->next;
    }

    return -1;
}

char* queue_peek(QUEUE* queue)
{
    char* value;
    QNODE* head;
    
    if (queue->head == NULL) {
        return NULL;
    }
    
    value = queue->head->value;
    head = queue->head->next;    
    free(queue->head);
    queue->head = head;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    
    return value;
    
}

void queue_dispose(QUEUE* queue)
{
    QNODE* curr;
    QNODE* pred;
    
    curr = queue->head;
    while (curr != NULL) {
        pred = curr;
        curr = curr->next;
        free(pred->value);
        free(pred);
    }
    
    queue->head = NULL;
    queue->tail = NULL;
}
