#include "tools.h"

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

std::string fix_addres(std::string path){
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
  std::string str;
  for(j = i; j < path.size(); j++){
    if(path[j] == '/')
      continue;
    str += path[j];
  }
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

std::string get_working_path(){
   char temp[256];
   return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}