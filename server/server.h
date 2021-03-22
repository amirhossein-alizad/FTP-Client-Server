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


#define REQUEST_ADDR "127.0.0.1"
#define DATA_ADDR "127.0.0.2"
#define CLIENT_COUNT 10


class  Server{
private:
  int clientSockets[10] = {0};
  int dataSockets[10] = {0};
  struct sockaddr_in request_serverAddr;
  struct sockaddr_in data_serverAddr;
  int request_channel;
  int data_channel;
  std::vector<User> users_list;
  std::vector<std::string> protected_files;
  std::ofstream logs;
  std::vector<std::string> login_commands{"pwd", "mkd", "dele", "ls", "cwd", "rename", "retr", "help", "quit"};
  std::vector<char*> help;


public:
  Server(){
    parse_json();
    read_help();
  }
  void parse_json();
  socketData handleIncomingConnections();
  void connectChannels(char* argv[]);
  void read_help();
  bool find_username(std::string );
  bool find_password(std::string , std::string, bool*);
  void handle_user(std::string*, bool*, int, std::vector<std::string>);
  void handleIncomingInformation(void* );
  void handle_pass(std::string, bool*, bool*, int, std::vector<std::string>, bool*);
  void handle_help(std::vector<std::string>, int, bool, bool);
  void handle_cwd(std::vector<std::string>, int, bool, bool, std::string*);
  void handle_pwd(std::vector<std::string>, std::string, int, bool, bool, std::string);
  void handle_mkd(std::vector<std::string>, int, bool, bool, std::string);
  void handle_dele(std::vector<std::string>, int, bool, bool, std::string, bool);
  void handle_ls(std::vector<std::string>, int, int, bool, bool, std::string);
  static std::vector<std::string> parse_command(char command[]){
      std::vector<std::string> parsed;
      std::string str;
      for(int i = 0; i < strlen(command); i++){
        if(command[i] == ' ' || command[i] == '\n'){
          parsed.push_back(str);
          str = "";
          continue;
        }
        str += command[i];
      }
      if(str != "")
        parsed.push_back(str);
      return parsed;
  }
};

char * str_to_charstar(std::string s);
static char *itoa_simple_helper(char *dest, int i);
char *itoa_simple(char *dest, int i);
bool file_exists(const char*);
std::vector<std::string> parse_msg(char* msg);
std::vector<std::string> ls(std::string);
std::string get_working_path();
std::string rmv_cwd(std::string path);
bool doesDirExist(std::string dir);
std::string move_back(std::string path);
std::string findFileName(std::string path);