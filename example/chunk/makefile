CC = gcc
SOURCE = ../../src/driver.c ../../src/hash.c ../../src/map.c ../../src/queue.c ../../src/lrucache.c ../../src/chunk.c  main.c
LIBS = ../../src

all: 
	gcc -I $(LIBS) $(SOURCE)  -o main 
clean:
	rm -f main *.db