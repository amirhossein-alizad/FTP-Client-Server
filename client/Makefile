CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: client

client: main.o client.o json.o User.o
	$(CC) main.o client.o json.o User.o $(FLAGS) -o client

main.o: main.cpp client.h
	$(CC) $(COMPILE_FLAGS) main.cpp

client.o: client.cpp client.h ../Json/json.h
	$(CC) $(COMPILE_FLAGS) client.cpp

json.o: ../Json/json.cpp ../Json/json.h ../server/User.h
	$(CC) $(COMPILE_FLAGS) ../Json/json.cpp

User.o: ../server/User.cpp ../server/User.h
	$(CC) $(COMPILE_FLAGS) ../server/User.cpp

.PHONY: clean

clean:
				rm *.o
				rm client
