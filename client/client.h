#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <string>
#include <fcntl.h>
#include <iostream>
#include "../Json/json.h"

#define REQUEST_ADDR "127.0.0.1"
#define DATA_ADDR "127.0.0.2"

char * str_to_charstar(std::string s);
void connectChannels(char* argv[]);
void informationHandle();
