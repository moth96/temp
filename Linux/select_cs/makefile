all:server client

server:server.o wrap.o
	gcc server.o wrap.o -o server -Wall

client:client.o wrap.o
	gcc client.o wrap.o -o client -Wall

server.o:server.c wrap.h
	gcc -c server.c -o server.o -Wall

client.o:client.c wrap.h
	gcc -c client.c -o client.o -Wall

wrap.o:wrap.c wrap.h
	gcc -c wrap.c -o wrap.o -Wall
