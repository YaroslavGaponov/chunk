#ifndef DRIVER_H
#define DRIVER_H

#include <stdio.h>

#define MARKER_END -1

typedef long OFFSET;

typedef struct HEADER
{
    long hash_table_size;
} HEADER;

typedef struct DRIVER
{
    FILE* fp;
    HEADER* header;
    OFFSET* htable;
} DRIVER;

int driver_create(char* filename, int hash_table_size);
int driver_open(DRIVER* driver, char* filename);
int driver_set(DRIVER* driver, char* key, char* value);
int driver_has(DRIVER* driver, char* key);
char* driver_get(DRIVER* driver, char* key);
int driver_remove(DRIVER* driver, char* key);
void driver_for_each(DRIVER* driver, int (*iterator)(char* key, char* value));
void driver_close(DRIVER* driver);

#endif