#include "server.h"

void Server::parse_json(){
  jsonParser();
  users_list = userGetter();
  protected_files = filesGetter();
  bool exists = file_exists("log.txt");
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

std::vector<std::string> Server::parse_command(char command[]){
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

socketData Server::handleIncomingConnections(){
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
    int t, i;
    socketData newSocket;
    for (i = 0; i < CLIENT_COUNT; i++)
    {
        if( clientSockets[i] == 0 && dataSockets[i] == 0)
        {
            clientSockets[i] = reqSocket;
            dataSockets[i] = dataSocket;
            newSocket.commandSocket = reqSocket;
            newSocket.dataSocket = dataSocket;
            logs<<"Adding to list of sockets as: "<<i<<std::endl;
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            logs<<"Time:\t"<<std::ctime(&now);
            break;
        }
    }
    logs<<"******************************************"<<std::endl;
    return (newSocket);
}

void Server::connectChannels(char* ports[]){
    if((request_channel = socket(AF_INET, SOCK_STREAM, 0)) < 0 
        || (data_channel = socket(AF_INET, SOCK_STREAM, 0 )) < 0)
    {
    std::cout<<"socket() failed"<<std::endl;
    exit(EXIT_FAILURE);
    }
    int opt = 1, opt2 = 1;
    if(setsockopt(request_channel, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0
        || setsockopt(data_channel, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt2, sizeof(opt2)) < 0)
    {
    std::cout<<"setsockopt() failed"<<std::endl;
    exit(EXIT_FAILURE);
    }

    memset(&request_serverAddr, 0, sizeof(request_serverAddr));
    memset(&data_serverAddr, 0, sizeof(data_serverAddr));

    request_serverAddr.sin_family = AF_INET;
    request_serverAddr.sin_port = htons(atoi(ports[0]));
    request_serverAddr.sin_addr.s_addr = inet_addr(REQUEST_ADDR);

    data_serverAddr.sin_family = AF_INET;
    data_serverAddr.sin_port = htons(atoi(ports[1]));
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

bool Server::find_username(std::string username){
  for(int i = 0; i < users_list.size(); i++)
    if(users_list[i].get_username() == username)
      return true;
  return false;
}

bool Server::find_password(std::string password, std::string username, bool* isAdmin){
  for(int i = 0; i < users_list.size(); i++)
    if(users_list[i].get_password() == password && users_list[i].get_username() == username){
      *isAdmin = users_list[i].is_admin();
      return true;
    }
  return false;
}

void Server::handle_user(std::string* loggedInUsername, bool* user, int commandSocket, std::vector<std::string> parsed){
  if(parsed.size() != 2){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    *user = false;
    *loggedInUsername = "";
  }
  else{
    if(find_username(parsed[1])){
      send(commandSocket, "331: User name okay, need password.", 36, 0);
      *user = true;
      *loggedInUsername = parsed[1];
    }
    else{
      send(commandSocket, "430: Invalid username or password.", 35, 0);
      *user = false;
      *loggedInUsername = "";
    }
  }
}

void Server::handleIncomingInformation(void* newSocket){
  bool user = false, pass = false;
  socketData * sock = (socketData *)newSocket;
  std::string loggedInUsername, directory = "./server";
  bool isAdmin = false;
  while(1){
      char * in = new char[256];
      recv(sock->commandSocket, in, 256, 0);
      std::vector<std::string> parsed = parse_command(in);
      if(parsed[0] == "user")
        handle_user(&loggedInUsername, &user, sock->commandSocket, parsed);
      else if(parsed[0] == "pass")
        handle_pass(loggedInUsername, &user, &pass, sock->commandSocket, parsed, &isAdmin);
      else if(parsed[0] == "help")
        handle_help(parsed, sock->commandSocket, user, pass);
      else if(parsed[0] == "pwd")
        handle_pwd(parsed, directory, sock->commandSocket, user, pass, directory);
      else if(parsed[0] == "cwd")
        handle_cwd(parsed, sock->commandSocket, user, pass, &directory);
      else if(parsed[0] == "mkd")
        handle_mkd(parsed, sock->commandSocket, user, pass, directory);
      else if(parsed[0] == "dele")
        handle_dele(parsed, sock->commandSocket, user, pass, directory, isAdmin);
      else if(parsed[0] == "ls")
        handle_ls(parsed, sock->commandSocket, sock->dataSocket, user, pass, directory);
      else if(parsed[0] == "quit")
        handle_quit(parsed, sock->commandSocket, &user, &pass, &directory, &isAdmin, &loggedInUsername);
      delete in;
  }
}

void Server::handle_pass(std::string username, bool* user, bool* pass, int commandSocket, std::vector<std::string> parsed, bool* isAdmin){
  if(parsed.size() != 2){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    *pass = false;
    return;
  }
  if(!(*user)){
    send(commandSocket, "503: Bad sequence of commands.", 31, 0);
    *pass = false;
  }
  else{
    if(find_password(parsed[1], username, isAdmin)){
      send(commandSocket, "230: User logged in, proceed. Logged out if appropriate.", 57, 0);
      *pass = true;
      
    }
    else{
      send(commandSocket, "430: Invalid username or password.", 35, 0);
      *pass = false;
    }
  }
}

void Server::handle_help(std::vector<std::string> parsed, int commandSocket, bool user, bool pass){// NOT WORKING 
  if(parsed.size() != 1){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  send(commandSocket, "214", 4, 0);
  std::ifstream fin;
  fin.open("help.txt");
  std::string str;
  while(getline(fin, str))
    send(commandSocket, str.c_str(), str.size(), 0);
  send(commandSocket, "DONE", 5, 0);
}

void Server::handle_pwd(std::vector<std::string> parsed, std::string directory, int commandSocket, bool user, bool pass, std::string path){
  if(parsed.size() != 1){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  path = "257: " + path;
  send(commandSocket, path.c_str(), path.size(), 0);
}

std::string get_working_path(){
   char temp[256];
   return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

void Server::handle_cwd(std::vector<std::string> parsed, int commandSocket, bool user, bool pass, std::string* cwd){
  if(parsed.size() < 1 || parsed.size() > 2){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  if(parsed.size() == 1 || (parsed[1] == ".." && *cwd == "./server"))
    *cwd = "./server";
  else if(parsed[1] == ".." && *cwd != "./server")
    *cwd = move_back(*cwd);
  else{
    std::string path = rmv_cwd(*cwd);
    path = get_working_path() + path + "/" + parsed[1];
    if(doesDirExist(path))
      *cwd = *cwd + "/" + parsed[1];
    else{
      send(commandSocket, "500: Error", 11, 0);
      return;
    }
  }
  send(commandSocket, "250: Successful change.", 24, 0);
}

void Server::handle_mkd(std::vector<std::string> parsed, int commandSocket, bool user, bool pass, std::string cwd){
  if(parsed.size() != 2){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  std::string temp1 = rmv_cwd(cwd);
  temp1 = get_working_path() + temp1 + "/" + parsed[1];
  std::string temp2 = rmv_cwd(cwd);
  temp2 = get_working_path() + temp2 + "/" + findDirectory(parsed[1]);
  if(doesDirExist(temp1) || !doesDirExist(temp2)){
    send(commandSocket, "500: Error", 11, 0);
    return;
  }
  std::string path = temp1;
  mkdir(path.c_str(), 0777);
  std::string msg = "257: " + cwd + "/" + parsed[1] + " created.";
  send(commandSocket, msg.c_str(), msg.size(), 0);
}

void Server::handle_dele(std::vector<std::string> parsed, int commandSocket, bool user, bool pass, std::string cwd, bool isAdmin){
  if(parsed.size() != 3){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  if(parsed[1] == "-d"){
    std::string path = rmv_cwd(cwd);
    path = get_working_path() + path + "/" + parsed[2];
    if(!doesDirExist(path)){
      send(commandSocket, "500: Error", 11, 0);
      return;
    }
    rmdir(path.c_str());
    std::string msg = "250: " + cwd + "/" + parsed[2] + " deleted.";
    send(commandSocket, msg.c_str(), msg.size(), 0);
  }
  else if(parsed[1] == "-f"){
    std::string path = rmv_cwd(cwd);
    path = get_working_path() + path + "/" + parsed[2];
    if(!file_exists(path.c_str())){
      send(commandSocket, "500: Error", 11, 0);
      return;
    }
    if(std::count(protected_files.begin(), protected_files.end(), findFileName(parsed[2])) && !isAdmin){
      send(commandSocket, "550: File unavailable.", 23, 0);
      return;
    }
    unlink(path.c_str());
    std::string msg = "250: " + cwd + "/" + parsed[2] + " deleted.";
    send(commandSocket, msg.c_str(), msg.size(), 0);
  }
  else send(commandSocket, "500: Error", 11, 0);
}

void Server::handle_ls(std::vector<std::string> parsed, int commandSocket, int dataSocket, bool user, bool pass, std::string cwd){
  if(parsed.size() != 1){
    send(commandSocket, "!", 2, 0);
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "!", 2, 0);
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  std::string path = get_working_path() + rmv_cwd(cwd);
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (path.c_str())) != NULL) {
    send(commandSocket, "#", 2, 0);
    while ((ent = readdir (dir)) != NULL){
      if(strcmp(ent->d_name, "..")==0 || strcmp(ent->d_name, ".") == 0)
        continue;
      send(dataSocket, ent->d_name, sizeof(ent->d_name), 0);
    }
    send(dataSocket, "#", 2, 0);
    send(commandSocket, "226: List transfer done.", 25, 0);
    closedir (dir);
  }
}

void Server::handle_quit(std::vector<std::string> parsed, int commandSocket, bool* user, bool* pass, std::string* directory,
                   bool* isAdmin, std::string *loggedInUsername)
{
  if(parsed.size() != 1){
    send(commandSocket, "501: Syntax error in parameters or arguments.", 46, 0);
    return;
  }
  if(!user || !pass){
    send(commandSocket, "332: Need account for login.", 29, 0);
    return;
  }
  *user = false; *pass = false; *isAdmin = false;
  *directory = "./server"; *loggedInUsername = "";
  send(commandSocket, "221: Successful Quit.", 22, 0);
}

char * str_to_charstar(std::string s){
  char * p = new char[s.length() + 1];
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

bool file_exists(const char* filename){
    std::ifstream infile(filename);
    return infile.good();
}

void Server::read_help(){
  std::ifstream fin;
  fin.open("help.txt");
  std::string str;
  while(getline(fin, str)){
    help.push_back(str_to_charstar(str));
  }
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

std::string rmv_cwd(std::string path){
  std::string str;
  int i = 0;
  while(str != "./server"){
    str += path[i];
    i++;
  }
  str = "";
  while(i < path.size()){
    str += path[i];
    i++;
  }
  return str;
}

bool doesDirExist(std::string dir){
  struct stat buffer;
  return (stat (dir.c_str(), &buffer) == 0);
}

std::string move_back(std::string path){
  int i = path.size() - 1;
  std::string newPath;
  while(path[i] != '/')
    i--;
  for(int j = 0; j < i; j++){
    newPath += path[j];
  }
  return newPath;
}

std::string findFileName(std::string path){
  int i = 0, j = 0;
  while(j < path.size()){
    if(path[j] == '/')
      i = j;
    j++;
  }
  i++;
  std::string str;
  for(j = i; j < path.size(); j++)
    str += path[j];
  return str;
}

std::string findDirectory(std::string path){
  int i = 0, j = 0;
  while(j < path.size()){
    if(path[j] == '/')
      i = j;
    j++;
  }
  std::string str;
  for(j = 0; j < i; j++)
    str += path[j];
  return str;
}