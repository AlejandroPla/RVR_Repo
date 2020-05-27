CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Chat.h XLDisplay.h
OBJ = Socket.o Chat.o XLDisplay.o
LIBS=-lpthread -lm -lX11

%.o: %.cc $(DEPS)
	$(CC) -std=c++11 -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) ChatServer.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc
