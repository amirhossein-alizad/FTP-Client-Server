#include "client.h"

int main(int argc, char *argv[])
{
    jsonParser();
    char** ports = new char*[2];
    ports[0] = str_to_charstar(getCommandPort());
    ports[1] = str_to_charstar(getDataPort());
    connectChannels(ports);
    while(1){
        informationHandle();
    }
}