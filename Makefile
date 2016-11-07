CC=clang++
DEBUGGER=lldb
LDFLAGS=-stdlib=libc++
CFLAGS=-std=c++1z
DEBUG= -g
SRC= test.cpp sha1.cpp
PROG= testWebsocket.out

default: 
	$(CC) $(CFLAGS) $(DEBUG) $(SRC) -o $(PROG) $(LDFLAGS)
	./$(PROG)

debug:
	$(CC) $(CFLAGS) $(DEBUG) $(SRC) -o $(PROG) $(LDFLAGS)
	$(DEBUGGER) ./$(PROG)
