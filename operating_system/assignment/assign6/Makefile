all:    server client

client: client.c
	# gcc -g client.c -o client -lsocket -lnsl
	gcc -g client.c -o client  -lnsl

server: server.c
	gcc -g server.c -o server  -lnsl
	#gcc -g server.c -o server -lsocket -lnsl

clean:
	rm client server

