CC = gcc
SOURCE = ../../src/driver.c ../../src/hash.c ../../src/map.c ../../src/queue.c ../../src/lrucache.c ../../src/chunk.c ../../src/storage.c main.c
LIBS = ../../src

all: 
	gcc -I $(LIBS) $(SOURCE)  -o main 
	mkdir ./db
clean:
	rm -r ./db
	rm -f main