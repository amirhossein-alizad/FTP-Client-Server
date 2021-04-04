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
    if(strcmp(in, "help") == 0){
        handle_help();
        return;
    }
    else if(strcmp(in, "ls") == 0){
        handle_ls();
        return;
    }
    else if(in[0] == 'r' && in[1] == 'e' && in[2] == 't' && in[3] == 'r' ){
        handle_dl();
        return;
    }
    char* msg = new char[256]; 
    recv(broadcastFD, msg, 256, 0);
    std::cout<<msg<<std::endl;
}


void handle_help(){
    char* msg = new char[256]; 
    recv(broadcastFD, msg, 256, 0);
    msg[strlen(msg)] = '\0';
    std::cout<<msg<<std::endl;
    if(strcmp(msg, "214") != 0){
        delete msg;
        return;
    }
    delete msg;
    char in[1012];
    recv(broadcastFD, in, 1012, 0);
    std::cout<<in<<std::endl;
}

void handle_dl(){
    char* result = new char[256];
    memset(result, 0, 256);
    recv(broadcastFD, result, 256, 0);
    if(result[0] == '!') {
        // char* temp = result + 1;
        std::cout << result+1 << std::endl;
        return;
    }

    char* msg = new char[50];
    memset(msg, 0, 50);
    recv(dataFD, msg, 50, 0);
    std::cout << msg;
    memset(msg, 0, 50);
    std::cout << "jajha\n";
    for(int i = 1; ; i++) {
        if(recv(dataFD, msg, 50, MSG_DONTWAIT) < 0)
            break;
        std::cout << i << std::endl;
        std::cout << msg;
        memset(msg, 0, 50); 
    }
    std::cout << std::endl;
    memset(msg, 0, 50); 
    recv(broadcastFD, msg, 50, 0);
    std::cout << msg << std::endl;
}

void handle_ls(){
    char* result = new char[2];
    memset(result, 0, 2);
    recv(broadcastFD, result, 2, 0);
    result[strlen(result)] = '\0';
    if(strcmp(result, "!") == 0){
        delete result;
        char* msg = new char[256];
        recv(broadcastFD, msg, 256, 0);
        std::cout<<msg<<std::endl;
        return;
    }

    char* msg = new char[256];
    memset(msg, 0, 256);
    recv(dataFD, msg, 256, 0);
    std::cout << msg;
    memset(msg, 0, 256);
    for(int i = 0; !(recv(dataFD, msg, 256, MSG_DONTWAIT) < 0); i++) {
        std::cout << msg;
        memset(msg, 0, 256); 
    }
    recv(broadcastFD, msg, 256, 0);
    std::cout << msg << std::endl; 
    // char* msg = new char[256]; 
    // recv(broadcastFD, msg, 256, 0);
    // msg[strlen(msg)] = '\0';
    // if(strcmp(msg, "!") == 0){
    //     delete msg;
    //     msg = new char[256];
    //     recv(broadcastFD, msg, 256, 0);
    //     std::cout<<msg<<std::endl;
    //     return;
    // }
    // while(1){
    //     delete msg;
    //     msg = new char[256];
    //     recv(dataFD, msg, 256, 0);
    //     msg[strlen(msg)] = '\0';
    //     if(strcmp(msg, "#") == 0){
    //         delete msg;
    //         msg = new char[256];
    //         recv(broadcastFD, msg, 256, 0);
    //         std::cout<<msg<<std::endl;
    //         return;
    //     }
    //     std::cout<<msg<<std::endl;
    // }
}