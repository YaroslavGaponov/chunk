#include <stdio.h>
#include "storage.h"

#define PATH "./db"
#define HTABLE_SIZE 9973
#define CACHE_SIZE 10
#define CHUNKS 5

int SIZE = 15;

int display (char* key, char* value)
{
    printf("storage_for_each %s = %s\n");
    return 0;
}

int main(int argc, char* argv[]) {
    int err, i;
    char key[10], value[10];    
    STORAGE* storage;
    
    /* create database file */
    err = storage_create(PATH, CHUNKS, HTABLE_SIZE);
    if (err) {
        printf("Error: cant create storage\n");
        return -1;
    }

    /* open database file */
    storage = malloc(sizeof(STORAGE));    
    err = storage_open(storage, PATH, CACHE_SIZE);
    if (err) {
        free(storage);
        printf("Error: problem with open storage\n");
        return -2;
    }    
exit(-1);
    /* insert  */
    for(i=0; i<SIZE; i++) {
        sprintf(key, "%s%d", "key", i);
        sprintf(value, "%s%d", "value", i);
        storage_set(storage, key, value);
    }

    /* remove */
    printf("storage_remove key5 %d\n", storage_remove(storage, "key5"));
    printf("storage_remove key7 %d\n", storage_remove(storage, "key7"));
    
    /* get */
    for(i=0; i<SIZE; i++) {
        sprintf(key, "%s%d", "key", i);
        printf("storage_get %s = %s\n",key, storage_get(storage, key));
    }    
    
    /* forEach */
    storage_for_each(storage, display);

    /* close */
    storage_close(storage);    
    free(storage);
    
    return  0;
}