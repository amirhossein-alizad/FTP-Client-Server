CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: server

server: main.o server.o json.o User.o
	$(CC) main.o server.o json.o User.o $(FLAGS) -o server

main.o: main.cpp server.h
	$(CC) $(COMPILE_FLAGS) main.cpp

server.o: server.cpp server.h json.h
	$(CC) $(COMPILE_FLAGS) server.cpp

json.o: json.cpp json.h User.h
	$(CC) $(COMPILE_FLAGS) json.cpp

User.o: User.cpp User.h
	$(CC) $(COMPILE_FLAGS) User.cpp

.PHONY: clean

clean:
				rm *.o
				rm server