#include "client.h"

int broadcastFD;
int dataFD;
struct sockaddr_in addressOfSocket;
struct sockaddr_in dataSocketAddr;

char * str_to_charstar(std::string s){
  char * p = new char[s.length() + 1];
  int i;
  for (i = 0; i < s.length(); i++)
      p[i] = s[i];
  p[s.length()] = '\0';
  return p;
}

void connectChannels(char* argv[]){
    if( (broadcastFD = socket( AF_INET , SOCK_STREAM, 0)) < 0 
        || (dataFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cout<<"failed to open socket"<<std::endl;
        exit(EXIT_FAILURE);
    }
    addressOfSocket.sin_family = AF_INET;
    addressOfSocket.sin_port = htons(atoi(argv[0]));
    addressOfSocket.sin_addr.s_addr = inet_addr(REQUEST_ADDR);
    dataSocketAddr.sin_family = AF_INET;
    dataSocketAddr.sin_port = htons(atoi(argv[1]));
    dataSocketAddr.sin_addr.s_addr = inet_addr(DATA_ADDR);
    if(connect(broadcastFD, (struct sockaddr *) &addressOfSocket, sizeof(addressOfSocket)) < 0
        || connect(dataFD, (struct sockaddr *) &dataSocketAddr, sizeof(dataSocketAddr)) < 0){
      std::cout<<"failed to connect"<<std::endl;
      exit(EXIT_FAILURE);
    }
    int port;
    char *in = new char[250];
    recv(broadcastFD, in, 250, 0);
    std::cout<<in<<std::endl;
    delete in;
    in = new char[250];
    recv(dataFD, in, 250, 0);
    std::cout<<in<<std::endl;
    delete in;
}

void informationHandle(){
    char in[256];
    std::cin.getline(in, 256);
    if(send(broadcastFD, in, sizeof(in), 0)<0)
        exit(EXIT_FAILURE);
    if(strcmp(in, "exit") == 0)
        exit(EXIT_SUCCESS);
    if(strcmp(in, "help") == 0)
        handle_help();
    else if(strcmp(in, "ls") == 0)
        handle_ls();
    char* msg = new char[256]; 
    recv(broadcastFD, msg, 256, 0);
    std::cout<<msg<<std::endl;
}


void handle_help(){
    char* msg = new char[256]; 
    recv(broadcastFD, msg, 256, 0);
    msg[strlen(msg)] = '\0';
    std::cout<<msg<<std::endl;
    if(strcmp(msg, "214") != 0)
        return;
    delete msg;
    while(1){
        char* newmsg = new char[512];
        recv(broadcastFD, newmsg, 512, 0);
        msg[strlen(newmsg)] = '\0';
        if(strcmp(newmsg, "DONE") == 0)
            return;
        std::cout<<newmsg<<std::endl;
        delete newmsg;
    }
}

void handle_ls(){
    char* msg = new char[256]; 
    recv(broadcastFD, msg, 256, 0);
    msg[strlen(msg)] = '\0';
    if(strcmp(msg, "!") == 0){
        delete msg;
        msg = new char[256];
        recv(broadcastFD, msg, 256, 0);
        std::cout<<msg<<std::endl;
        return;
    }
    while(1){
        delete msg;
        msg = new char[256];
        recv(dataFD, msg, 256, 0);
        msg[strlen(msg)] = '\0';
        if(strcmp(msg, "#") == 0){
            delete msg;
            msg = new char[256];
            recv(broadcastFD, msg, 256, 0);
            std::cout<<msg<<std::endl;
            return;
        }
        std::cout<<msg<<std::endl;
    }
}