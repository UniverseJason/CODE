CC = gcc
CFLAGS = -pthread

all: prog Main

prog: thdSort.c
	$(CC) $(CFLAGS) thdSort.c -o prog

Main: Main.java
	javac Main.java

clean:
	rm -f *.o prog
	rm -f *.class