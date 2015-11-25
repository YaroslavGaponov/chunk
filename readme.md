# Chunk

    NoSQL database in pure ANSI C

## Usage

###### code

```c
#include <stdio.h>
#include "chunk.h"

#define FILENAME "chunk.db"
#define HTABLE_SIZE 9973
#define CACHE_SIZE  10

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
```

Result
```output
[gap@localhost example]$ make
gcc -I ../src ../src/driver.c ../src/hash.c ../src/map.c ../src/queue.c ../src/lrucache.c ../src/chunk.c main.c  -o main 
[gap@localhost example]$ ./main 
chunk_remove key5 0
chunk_remove key7 0
chunk_get key0 = value0
chunk_get key1 = value1
chunk_get key2 = value2
chunk_get key3 = value3
chunk_get key4 = value4
chunk_get key5 = (null)
chunk_get key6 = value6
chunk_get key7 = (null)
chunk_get key8 = value8
chunk_get key9 = value9
chunk_get key10 = value10
chunk_get key11 = value11
chunk_get key12 = value12
chunk_get key13 = value13
chunk_get key14 = value14
chunk_for_each value10 = value10
chunk_for_each value11 = value11
chunk_for_each value12 = value12
chunk_for_each value13 = value13
chunk_for_each value14 = value14
chunk_for_each value0 = value0
chunk_for_each value1 = value1
chunk_for_each value2 = value2
chunk_for_each value3 = value3
chunk_for_each value4 = value4
chunk_for_each value6 = value6
chunk_for_each value8 = value8
chunk_for_each value9 = value9
[gap@localhost example]$ 

```

