CC = gcc
CFLAGS = -Wall -g

all: wordcount multiple_wordcount

clean:
	rm -f *.o wordcount multiple_wordcount

wordcount: wordcount.c
	$(CC) $(CFLAGS) wordcount.c -o wordcount

multiple_wordcount: wordcount.c
	$(CC) $(CFLAGS) multiple_wordcount.c -o multiple_wordcount

