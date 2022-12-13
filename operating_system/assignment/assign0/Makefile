CC = gcc
CFLAGS = -Wall -g

all: driver1 

clean:
	rm -f *.o driver1 

mylinkedlist.o: mylinkedlist.c mylinkedlist.h
	$(CC) $(CFLAGS) -c mylinkedlist.c

driver1.o: driver1.c mylinkedlist.h
	$(CC) $(CFLAGS) -c driver1.c

driver1: driver1.o mylinkedlist.o
	$(CC) $(CFLAGS) -o driver1 driver1.o mylinkedlist.o

