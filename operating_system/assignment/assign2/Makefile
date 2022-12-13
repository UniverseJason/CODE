CC = gcc
CFLAGS = -Wall -g

all: prog

clean:
	rm -f prog *.o

progFunction.o: progFunction.c progFunction.h
	$(CC) $(CFLAGS) -c progFunction.c

prog.o: prog.c progFunction.h
	$(CC) $(CFLAGS) -c prog.c

prog: progFunction.o prog.o
	$(CC) $(CFLAGS) -o prog prog.o progFunction.o

run1:
	prog -alg FIFO -input input1.txt
run2:
	prog -alg SJF -input input1.txt
run3:
	prog -alg PR -input input1.txt
run4:
	prog -alg RR -quantum 3  -input input1.txt