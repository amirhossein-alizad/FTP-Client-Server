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
#include <chrono>
#include <ctime> 
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <algorithm>
#include "../Json/json.h"
#include "socketData.h"

char * str_to_charstar(std::string s);

static char *itoa_simple_helper(char *dest, int i);

char *itoa_simple(char *dest, int i);

bool file_exists(const char*);

std::vector<std::string> parse_msg(char* msg);

std::vector<std::string> ls(std::string);

std::string get_working_path();

std::string fix_addres(std::string path);

bool doesDirExist(std::string dir);

std::string move_back(std::string path);

std::string findFileName(std::string path);

std::string findDirectory(std::string path);