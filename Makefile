CC=g++
OPTS=-std=c++0x -Werror -Wall -Wextra
OPTS2=-std=c++0x -Wall -Wextra -lpthread


singleThreadR:
	$(CC) $(OPTS) -o singleThreadR.o singleThread.cpp

singleThreadRW:
	$(CC) $(OPTS) -o singleThreadRW.o singleThread_RW.cpp

multiThreadR:
	$(CC) $(OPTS2) -o multiThreadR.o multiThread.cpp

multiThreadRW:
	$(CC) $(OPTS2) -o multiThreadRW.o multiThread_RW.cpp

clean:
	rm -f *.o

all:
	$(singleThreadR) $(singleThreadRW) $(multiThreadR) $(multiThreadRW)
