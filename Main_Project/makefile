CFLAGS=-Wall -Wextra -std=c11 -O2
CC=gcc

all: quantization

quantization: main.o charvector.o find_and_union.o input.o logic.o trie.o
	$(CC) $(CFLAGS) -o quantization main.o charvector.o find_and_union.o input.o logic.o trie.o

charvector.o: charvector.c charvector.h constants.h
	$(CC) $(CFLAGS) -c charvector.c

find_and_union.o: find_and_union.c  find_and_union.h
	$(CC) $(CFLAGS) -c find_and_union.c

find_and_union.o: find_and_union.c  find_and_union.h
	$(CC) $(CFLAGS) -c find_and_union.c

input.o: input.c input.h charvector.h
	$(CC) $(CFLAGS) -c input.c

logic.o: logic.c logic.h trie.h constants.h charvector.h 
	$(CC) $(CFLAGS) -c logic.c

trie.o: trie.c trie.h find_and_union.h
	$(CC) $(CFLAGS) -c trie.c

clean:
	rm *.o quantization
