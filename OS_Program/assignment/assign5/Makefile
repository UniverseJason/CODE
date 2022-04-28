CC = gcc
CFLAGS = -Wall -g

all: prog

clean:
	rm -f prog *.o

progFunctionThd.o: progFunctionThd.c progFunctionThd.h
	$(CC) $(CFLAGS) -c progFunctionThd.c

prog.o: prog.c progFunctionThd.h
	$(CC) $(CFLAGS) -c prog.c

prog: progFunctionThd.o prog.o
	$(CC) $(CFLAGS) -o prog prog.o progFunctionThd.o