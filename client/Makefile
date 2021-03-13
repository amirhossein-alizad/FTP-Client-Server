CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: client

client: main.o client.o
	$(CC) main.o client.o $(FLAGS) -o client

main.o: main.cpp client.h
	$(CC) $(COMPILE_FLAGS) main.cpp

client.o: client.cpp client.h
	$(CC) $(COMPILE_FLAGS) client.cpp

.PHONY: clean

clean:
				rm *.o
				rm client
