#include "tools.h"

#define REQUEST_ADDR "127.0.0.1"
#define DATA_ADDR "127.0.0.2"
#define CLIENT_COUNT 50

class  Server{
  private:
    int clientSockets[CLIENT_COUNT] = {0};
    int dataSockets[CLIENT_COUNT] = {0};
    struct sockaddr_in request_serverAddr;
    struct sockaddr_in data_serverAddr;
    int request_channel;
    int data_channel;
    std::vector<User> users_list;
    std::vector<std::string> protected_files;
    std::ofstream logs;
    std::vector<std::string> login_commands{"pwd", "mkd", "dele", "ls", "cwd", "rename", "retr", "help", "quit"};
    std::vector<std::string> help;

  public:
    Server(){
      parse_json();
    }
    void parse_json();
    socketData handleIncomingConnections();
    void connectChannels(char* argv[]);
    void send_help(int);
    bool find_username(std::string );
    bool find_password(std::string , std::string, bool*);
    void handle_user(std::string*, bool*, bool, int, int, std::vector<std::string>);
    void handleIncomingInformation(void* );
    void handle_pass(std::string, bool*, bool*, int, int, std::vector<std::string>, bool*);
    void handle_help(std::vector<std::string>, int, int, bool, bool, std::string);
    void handle_cwd(std::vector<std::string>, int, int, bool, bool, std::string*, std::string);
    void handle_pwd(std::vector<std::string>, std::string, int, int, bool, bool, std::string, std::string);
    void handle_mkd(std::vector<std::string>, int, int, bool, bool, std::string, std::string);
    void handle_dele(std::vector<std::string>, int, int, bool, bool, std::string, bool, std::string);
    void handle_ls(std::vector<std::string>, int, int, bool, bool, std::string, std::string);
    void handle_dl(std::vector<std::string>, int, int, bool, bool, bool, std::string, std::string);
    std::vector<std::string> parse_command(char[]);
    void handle_quit(std::vector<std::string>, int, int, bool*, bool*, std::string*,bool*, std::string*);
    void printTime();
    void printLoginError(std::string, int, int);
    void printSyntaxError(std::string, int, int, std::string);
    void handle_rename(std::vector<std::string>, int, int, bool, bool, bool, std::string, std::string);
    void handle_error(int, int);
    std::string checkForServer(std::string cwd, bool*);
};