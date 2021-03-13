#include "server.h"

int main(int argc, char *argv[])
{
  parse_json();
  connectChannels(argv);
  while(1)
  {
    handleSelect();
    handleIncomingConnections();
    handleIncomingInformation();
  }
}
