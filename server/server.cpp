#include "server.h"

int clientSockets[50] = {0};
int dataSockets[50] = {0};
fd_set requestFDs;
fd_set dataFDs;
struct sockaddr_in request_serverAddr;
struct sockaddr_in data_serverAddr;
int request_channel;
int data_channel;
std::vector<User> users_list;
std::vector<std::string> protected_files;
std::ofstream logs;
char * str_to_charstar(std::string s){
  char * p = new char[s.length()];
  int i;
  for (i = 0; i < s.length(); i++)
      p[i] = s[i];
  p[s.length()] ='\0';
  return p;
}

static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}

char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}

void connectChannels(char* argv[]){
    if((request_channel = socket(AF_INET, SOCK_STREAM, 0)) < 0 
        || (data_channel = socket(AF_INET, SOCK_STREAM, 0 )) < 0)
    {
      std::cout<<"socket() failed"<<std::endl;
      exit(EXIT_FAILURE);
    }
    int opt = 1, opt2 = 1;
    if(setsockopt(request_channel, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0
        || setsockopt(data_channel, SOL_SOCKET, SO_REUSEADDR, (char *)&opt2, sizeof(opt2)) < 0)
    {
      std::cout<<"setsockopt() failed"<<std::endl;
      exit(EXIT_FAILURE);
    }

    memset(&request_serverAddr, 0, sizeof(request_serverAddr));
    memset(&data_serverAddr, 0, sizeof(data_serverAddr));

    request_serverAddr.sin_family = AF_INET;
    request_serverAddr.sin_port = htons(atoi(argv[1]));
    request_serverAddr.sin_addr.s_addr = inet_addr(REQUEST_ADDR);

    data_serverAddr.sin_family = AF_INET;
    data_serverAddr.sin_port = htons(atoi(argv[1]));
    data_serverAddr.sin_addr.s_addr = inet_addr(DATA_ADDR);
    if (bind(request_channel, (struct sockaddr *) &request_serverAddr, sizeof(request_serverAddr)) < 0
        || bind(data_channel, (struct sockaddr *) &data_serverAddr, sizeof(data_serverAddr)) < 0)
    {
      std::cout<<"bind() failed"<<std::endl;
      exit(EXIT_FAILURE);
    }
    if(listen(request_channel, CLIENT_COUNT) < 0
        || listen(data_channel, CLIENT_COUNT) < 0)
    {
      std::cout<<"listen() failed"<<std::endl;
      exit(EXIT_FAILURE);
    }
}

void handleSelect(){
    FD_ZERO(&requestFDs);
    FD_ZERO(&dataFDs);
    FD_SET(request_channel, &requestFDs);
    FD_SET(data_channel, &dataFDs);
    int highestFileDescriptor = CLIENT_COUNT;
    for (int  i = 0 ; i < CLIENT_COUNT ; i++)
    {
        if(clientSockets[i] > 0){
            FD_SET(clientSockets[i], &requestFDs);
            FD_SET(dataSockets[i], &dataFDs);
        }
        if(clientSockets[i] > highestFileDescriptor)
            highestFileDescriptor = clientSockets[i];
        if(dataSockets[i] > highestFileDescriptor)
          highestFileDescriptor = dataSockets[i];
    }
    int s = select( highestFileDescriptor+1, &requestFDs, NULL , NULL , NULL);
    int s2 = select( highestFileDescriptor+1, &dataFDs, NULL , NULL , NULL);
    if ((s < 0 || s2 < 0)  &&  (errno!=EINTR))
        std::cout<<"select error"<<std::endl;
}


void handleIncomingConnections(){
    int reqSocket;
    int dataSocket;
    int addrlen;
    int addrlen2;
    struct sockaddr_in reqaddress;
	  memset(&reqaddress, 0, sizeof(reqaddress));
    struct sockaddr_in dataaddress;
	  memset(&dataaddress, 0, sizeof(dataaddress));
    addrlen = sizeof(reqaddress);
    addrlen2 = sizeof(dataaddress);
    if (FD_ISSET(request_channel, &requestFDs) && FD_ISSET(data_channel, &dataFDs))        
    {
        if ((reqSocket= accept(request_channel, (struct sockaddr *)&reqaddress, (socklen_t*)&addrlen)) < 0)
        {
            std::cout<<"accept"<<std::endl;
            exit(EXIT_FAILURE);
        }
        char* addr = inet_ntoa(reqaddress.sin_addr);
        char port[256]; 
        itoa_simple(port, ntohs(reqaddress.sin_port));
        std::string msg  = "$YOU ARE NOW CONNECTED TO REQUEST CHANNEL";
        logs<<"New Connection in request channel, address :127.0.0.1:"<<port<<std::endl;
        send(reqSocket, str_to_charstar(msg), 42, 0);
        if((dataSocket = accept(data_channel, (struct sockaddr *)&dataaddress, (socklen_t*)&addrlen2)) < 0){
            std::cout<<"accept"<<std::endl;
            exit(EXIT_FAILURE);
        }
        addr = inet_ntoa(dataaddress.sin_addr);
        port[256]; 
        itoa_simple(port, ntohs(dataaddress.sin_port));
        msg  = "$YOU ARE NOW CONNECTED TO DATA CHANNEL";
        logs<<"New Connection in data channel, address :127.0.0.2:"<<port<<std::endl;
        send(dataSocket, str_to_charstar(msg), 39, 0);
        int i;
        for (i = 0; i < CLIENT_COUNT; i++)
        {
            if( clientSockets[i] == 0 && dataSockets[i] == 0)
            {
                clientSockets[i] = reqSocket;
                dataSockets[i] = dataSocket;
                logs<<"Adding to list of sockets as: "<<i<<std::endl;
                std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                logs<<"Time:\t"<<std::ctime(&now);
                break;
            }
        }
        logs<<"******************************************"<<std::endl;
    }
}

void handleIncomingInformation(){
    for(int i = 0; i< CLIENT_COUNT; i++){
      if(FD_ISSET(clientSockets[i], &requestFDs) && FD_ISSET(dataSockets[i], &dataFDs)){
        char *message = new char[250];
        recv(clientSockets[i], message, 250, 0);
        std::cout<<strlen(message)<<std::endl;
        std::cout<<message<<std::endl;
        close(clientSockets[i]);
        close(dataSockets[i]);
        dataSockets[i] = 0;
        clientSockets[i] = 0;
        delete message;
      }
    }
}

void parse_json(){
  jsonParser();
  users_list = userGetter();
  protected_files = filesGetter();
  bool exists = file_exists();
  logs.open("log.txt", std::ios_base::app);
  if(!exists){
    logs<<"Users:"<<std::endl;
    for(int i = 0; i < users_list.size(); i++){
        logs<<"\tusername: "<<users_list[i].get_username()<<std::endl;
        logs<<"\tpassword: "<<users_list[i].get_password()<<std::endl;
        logs<<"\tadmin: "<<(users_list[i].is_admin())? "true" : "false";logs<<std::endl;
        logs<<"\tsize: "<<users_list[i].get_size()<<std::endl;
        logs<<"******************************************"<<std::endl;
        
    }
    logs<<"Files:"<<std::endl;
    for(int i = 0; i < protected_files.size(); i++)
        logs<<"\t"<<protected_files[i]<<std::endl;
    logs<<"******************************************"<<std::endl;
  }
}

bool file_exists(){
    std::ifstream infile("log.txt");
    return infile.good();
}

std::vector<std::string> parse_msg(char* msg){
  std::string str;
  std::vector<std::string> parsed;
  for(int i = 0; i < strlen(msg); i++){
    if(msg[i] == ' '){
      parsed.push_back(str);
      str = "";
      continue;
    }
    str += msg[i];
  }
  if(str != "")
    parsed.push_back(str);
  return parsed;
}