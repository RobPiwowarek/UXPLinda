#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    Client client;

    sleep(5);

    std::cout << "Client rand output (5)" << std::endl;
    bool success = client.output("(5)");
}