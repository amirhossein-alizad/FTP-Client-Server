CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: server

server: main.o server.o json.o User.o tools.o
	$(CC) main.o server.o json.o User.o tools.o $(FLAGS) -o server -lpthread

main.o: main.cpp server.h
	$(CC) $(COMPILE_FLAGS) main.cpp

server.o: server.cpp server.h ../Json/json.h socketData.h tools.h
	$(CC) $(COMPILE_FLAGS) server.cpp

json.o: ../Json/json.cpp ../Json/json.h User.h
	$(CC) $(COMPILE_FLAGS) ../Json/json.cpp

User.o: User.cpp User.h
	$(CC) $(COMPILE_FLAGS) User.cpp

tools.o: tools.cpp tools.h
	$(CC) $(COMPILE_FLAGS) tools.cpp

.PHONY: clean

clean:
				rm *.o
				rm server
				rm log.txt