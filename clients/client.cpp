#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec= 0;

    Client client;

    std::string tuple=client.input("Integer:*",&timeout);

    std::cout<< "Client received tuple: " <<tuple<<std::endl;

    return EXIT_SUCCESS;
}