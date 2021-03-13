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
    addressOfSocket.sin_port = htons(atoi(argv[1]));
    addressOfSocket.sin_addr.s_addr = inet_addr(REQUEST_ADDR);
    dataSocketAddr.sin_family = AF_INET;
    dataSocketAddr.sin_port = htons(atoi(argv[1]));
    dataSocketAddr.sin_addr.s_addr = inet_addr(DATA_ADDR);
    if(connect(broadcastFD, (struct sockaddr *) &addressOfSocket, sizeof(addressOfSocket)) < 0
        || connect(dataFD, (struct sockaddr *) &dataSocketAddr, sizeof(dataSocketAddr)) < 0){
      std::cout<<"failed to connect"<<std::endl;
      exit(EXIT_FAILURE);
    }
}

void informationHandle(){
    int port;
    char *in = new char[250];
    recv(broadcastFD, in, 250, 0);
    std::cout<<in<<std::endl;
    delete in;
    in = new char[250];
    recv(dataFD, in, 250, 0);
    char input[256];
    std::cout<<in<<std::endl;
    std::cin.getline(input,sizeof(input));
    delete in;
    send(broadcastFD, input, sizeof(input), 0);
    close(broadcastFD);
}
