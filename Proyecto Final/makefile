CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Message.h Client.h Server.h Game.h GameObjects.h XLDisplay.h
OBJ  = Socket.o Message.o Client.o Server.o Game.o GameObjects.o XLDisplay.o
LIBS=-lpthread -lm -lX11

%.o: %.cc $(DEPS)
	$(CC) -std=c++11 -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) GameServer.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) GameClient.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc