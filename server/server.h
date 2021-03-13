#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "json.h"
#include <chrono>
#include <ctime> 

#define REQUEST_ADDR "127.0.0.1"
#define DATA_ADDR "127.0.0.2"
#define CLIENT_COUNT 50

char * str_to_charstar(std::string s);
static char *itoa_simple_helper(char *dest, int i);
char *itoa_simple(char *dest, int i);
void connectChannels(char* argv[]);
void handleSelect();
void handleIncomingConnections();
void handleIncomingInformation();
void parse_json();
bool file_exists();
std::vector<std::string> parse_msg(char* msg);