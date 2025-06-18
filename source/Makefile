CC=gcc
OBJ=erproc.o

all: server

server: server.o $(OBJ)
	$(CC) -o server server.o $(OBJ)

server.o: server.c erproc.h
	$(CC) -c server.c

erproc.o: erproc.c erproc.h
	$(CC) -c erproc.c

clear:
	rm -f *.o server
