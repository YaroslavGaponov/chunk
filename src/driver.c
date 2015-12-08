#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "driver.h"
#include "hash.h"

typedef struct RECORD
{
    char* key;
    char* value;
    OFFSET next;
} RECORD;

static int _driver_format(FILE* fp, int hash_table_size)
{
    int i;
    HEADER* header;
    OFFSET* htable;    
    
    header = malloc(sizeof(HEADER));
    if (header == NULL) {
        return -1;
    }    
    memcpy(header->sign, "CHUNK", 5);
    header->hash_table_size = hash_table_size;
    header->size = 0L;
    fseek(fp, 0L, SEEK_SET);
    fwrite(header, sizeof(HEADER), 1, fp);    
    free(header);
    
    htable = malloc(sizeof(OFFSET) * hash_table_size);    
    if (htable == NULL) {
        return -2;
    }
    for(i = 0; i < hash_table_size; i++) {
        htable[i] = MARKER_END;
    }
    
    fseek(fp, sizeof(HEADER), SEEK_SET);
    fwrite(htable, sizeof(OFFSET), hash_table_size, fp);
    free(htable);
    
    fflush(fp);
    
    return 0;
}

static int _driver_save_size(FILE* fp, long size)
{
    fseek(fp, 5L ,SEEK_SET);
    fwrite(&size, sizeof(long), 1, fp);
    fflush(fp);
    
    return 0;
}

static RECORD* _driver_load(FILE* fp, OFFSET offset)
{
    int key_size;
    int value_size;
    RECORD* rec;
    
    rec = malloc(sizeof(RECORD));    
    fseek(fp, offset, SEEK_SET);        
    fread(&key_size, sizeof(int), 1, fp);
    fread(&value_size, sizeof(int), 1, fp);            
    rec->key = malloc(key_size);
    fread(rec->key, key_size, 1, fp);            
    rec->value = malloc(value_size);
    fread(rec->value, value_size, 1, fp);            
    fread(&rec->next, sizeof(OFFSET),1, fp);
    
    return rec;
}

static OFFSET _driver_save(FILE* fp, RECORD* rec)
{        
    int key_size;
    int value_size;
    OFFSET offset;

    fseek(fp, 0L, SEEK_END);
    offset = ftell(fp);
    key_size = strlen(rec->key);
    fwrite(&key_size, sizeof(int), 1, fp);
    value_size = strlen(rec->value);
    fwrite(&value_size, sizeof(int), 1, fp);
    fwrite(rec->key, key_size, 1, fp);
    fwrite(rec->value, value_size, 1, fp);
    fwrite(&rec->next, sizeof(OFFSET), 1, fp);    
    fflush(fp);
    
    return offset;
}

static int _driver_update_first(FILE* fp, int indx, OFFSET next)
{
    fseek(fp, sizeof(HEADER) + sizeof(OFFSET)*  indx, SEEK_SET);
    fwrite(&next, sizeof(OFFSET), 1, fp);
    fflush(fp);
    
    return 0;    
}

static int _driver_update_next(FILE* fp, OFFSET offset, OFFSET next)
{
    int key_size;
    int value_size;
    
    fseek(fp, offset, SEEK_SET);        
    fread(&key_size, sizeof(int), 1, fp);
    fread(&value_size, sizeof(int), 1, fp);                
    fseek(fp, key_size + value_size, SEEK_CUR);
    fwrite(&next, sizeof(OFFSET), 1, fp);
    fflush(fp);
    
    return 0;    
}

int driver_create(char* filename, int hash_table_size)
{
    FILE* fp;
    int err;
    
    fp = fopen (filename, "w+");
    if (fp == NULL) {
        return errno;
    }
    
    err = _driver_format(fp, hash_table_size);
    
    fclose(fp);
    
    if (err) {
        return errno;
    }
    
    return 0;
}

int driver_open(DRIVER* driver, char* filename)
{
    FILE* fp;

    fp = fopen (filename, "r+");
    if (fp == NULL) {
        return errno;
    }
    
    flockfile(fp);
    
    driver->fp = fp;
    driver->header = malloc(sizeof(HEADER));        
    fseek(driver->fp, 0L, SEEK_SET);
    fread(driver->header, sizeof(HEADER), 1, driver->fp);

    if (strcmp(driver->header->sign, "CHUNK") != 0) {
        return -1;
    }
    
    driver->htable = malloc(sizeof(OFFSET) *  driver->header->hash_table_size);
    fseek(driver->fp, sizeof(HEADER), SEEK_SET);
    fread(driver->htable, sizeof(OFFSET), driver->header->hash_table_size, driver->fp);
    
    return 0;
}


int driver_set(DRIVER* driver, char* key, char* value)
{
    int indx;
    int cmp;
    OFFSET curr_offset;
    OFFSET prev_offset;
    OFFSET new_offset;
    RECORD* curr_rec;
    RECORD new_rec;
    
    new_rec.key = malloc(sizeof(char)*  strlen(key));
    new_rec.key = key;
    new_rec.value = value;
    new_rec.next = MARKER_END;

    indx = hash_get(key) % driver->header->hash_table_size;
    curr_offset = driver->htable[indx];

    if (curr_offset == MARKER_END) {
        new_offset = _driver_save(driver->fp, &new_rec);    
        driver->htable[indx] = new_offset;
        _driver_update_first(driver->fp, indx, new_offset);
        _driver_save_size(driver->fp, ++driver->header->size);
        return 0;
    }
    
    prev_offset = MARKER_END;
    do {
        curr_rec = _driver_load(driver->fp, curr_offset);
        
        cmp = strcmp(curr_rec->key, key);
        if (cmp == 0) {
            new_rec.next = curr_rec->next;
            new_offset = _driver_save(driver->fp, &new_rec);            
            if (prev_offset == MARKER_END) {                                
                driver->htable[indx] = new_offset;
                _driver_update_first(driver->fp, indx, new_offset);
            } else {
                _driver_update_next(driver->fp, prev_offset, new_offset);
            }
            return 0;
        }
        if (cmp > 0) {
            new_rec.next = curr_offset;
            new_offset = _driver_save(driver->fp, &new_rec);
            _driver_save_size(driver->fp, ++driver->header->size);
            if (prev_offset == MARKER_END) {                                
                driver->htable[indx] = new_offset;
                _driver_update_first(driver->fp, indx, new_offset);
            } else {
                _driver_update_next(driver->fp, prev_offset, new_offset);
            }
            return 0;
        }
        
        prev_offset = curr_offset;
        curr_offset = curr_rec->next;
        
    } while (curr_offset != MARKER_END);
    
    new_offset = _driver_save(driver->fp, &new_rec);
    _driver_update_next(driver->fp, prev_offset, new_offset);
    _driver_save_size(driver->fp, ++driver->header->size);

    return 0;
}

int driver_has(DRIVER* driver, char* key)
{
    if (driver_get(driver, key) == NULL) {
        return 0;
    }
    return 1;
}

char* driver_get(DRIVER* driver, char* key)
{
    RECORD* rec;
    OFFSET offset;
    int indx;    
    int cmp;
        
    indx = hash_get(key) % driver->header->hash_table_size;    
    offset = driver->htable[indx];    
    while (offset != MARKER_END) {
        rec = _driver_load(driver->fp, offset);
        cmp = strcmp(rec->key, key);
        if (cmp == 0) {
            return rec->value;
        }
        if (cmp > 0) {
            return NULL;
        }
        offset = rec->next;
    }
    
    return NULL;
}

int driver_remove(DRIVER* driver, char* key)
{
    RECORD* rec;
    OFFSET curr;
    OFFSET prev;
    int indx;
    int cmp;    
    
    indx = hash_get(key) % driver->header->hash_table_size;
    
    curr = driver->htable[indx];
    prev = MARKER_END;
    
    while (curr != MARKER_END) {
        rec = _driver_load(driver->fp, curr);
        cmp = strcmp(rec->key, key);
        if (cmp == 0) {
            if (prev == MARKER_END) {
                driver->htable[indx] = rec->next;
                _driver_update_first(driver->fp, indx, driver->htable[indx]);
            } else {
                _driver_update_next(driver->fp, prev, rec->next);
            }
            _driver_save_size(driver->fp, --driver->header->size);
            return 0;
        }
        if (cmp > 0) {
            return 0;
        }
        prev = curr;
        curr = rec->next;        
    }
    
    return 0;
}

void driver_close(DRIVER* driver)
{
    funlockfile(driver->fp);
    
    fclose(driver->fp);
    
    free(driver->htable);
    free(driver->header);
}

void driver_for_each(DRIVER* driver, int (*iterator)(char* key, char* value))
{
    int i;
    int done;
    OFFSET offset;    
    RECORD* rec;

    for(i = 0; i < driver->header->hash_table_size; i++) {
        offset = driver->htable[i];
        while (offset != MARKER_END) {            
            rec = _driver_load(driver->fp, offset);
            done = iterator(rec->key, rec->value);
            if (done) {
                return;
            }
            offset = rec->next;
        }
    }
}

