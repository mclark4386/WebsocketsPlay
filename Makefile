CC=clang++
LDFLAGS=-stdlib=libc++
CFLAGS=-std=c++1z
DEBUG= -g
SRC= test.cpp
PROG= testWebsocket.out

default: 
	$(CC) $(CFLAGS) $(DEBUG) $(SRC) -o $(PROG) $(LDFLAGS)
	./$(PROG)
