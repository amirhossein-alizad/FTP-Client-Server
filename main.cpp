#include "server.h"
#include <sys/wait.h>

int main(int argc, char *argv[]){
  Server server;
  char ** ports = new char*[2];
  ports[0] = str_to_charstar(getCommandPort());
  ports[1] = str_to_charstar(getDataPort());
  server.connectChannels(ports);
  while(1){
    socketData sock = server.handleIncomingConnections();
    int fork_c = fork();
    if(fork_c == 0){
      server.handleIncomingInformation(static_cast<void*> (&sock));
      break;
    }
  }
  wait(NULL);
  
}
