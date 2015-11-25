#include <stdio.h>
#include "chunk.h"

#define FILENAME "chunk.db"
#define HTABLE_SIZE 9973
#define CACHE_SIZE 10

int SIZE = 15;

int display (char *key, char *value)
{
    printf("chunk_for_each %s = %s\n");
    return 1;
}

int main(int argc, char *argv[]) {

    int err, i;
    char key[10], value[10];    
    CHUNK *chunk;
    
    /* create database file */
    err = chunk_create(FILENAME, HTABLE_SIZE);
    if (err) {
        printf("Error: cant create chunk file\n");
        return -1;
    }

    /* open database file */
    chunk = malloc(sizeof(CHUNK));    
    err = chunk_open(chunk, FILENAME, CACHE_SIZE);
    if (err) {
        free(chunk);
        printf("Error: problem with open chunk file\n");
        return -2;
    }

    /* insert  */
    for(i=0; i<SIZE; i++) {
        sprintf(key, "%s%d", "key", i);
        sprintf(value, "%s%d", "value", i);
        chunk_set(chunk, key, value);
    }

    /* remove */
    printf("chunk_remove key5 %d\n", chunk_remove(chunk, "key5"));
    printf("chunk_remove key7 %d\n", chunk_remove(chunk, "key7"));
    
    /* get */
    for(i=0; i<SIZE; i++) {
        sprintf(key, "%s%d", "key", i);
        printf("chunk_get %s = %s\n",key, chunk_get(chunk, key));
    }    
    
    /* forEach */
    chunk_for_each(chunk, display);

    /* close */
    chunk_close(chunk);    
    free(chunk);
    
    return  0;
}