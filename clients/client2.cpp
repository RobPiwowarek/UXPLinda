#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec= 0;

    Client client;

    bool success = client.output("(5)");

    std::cout<< "Success in sending" <<success<<std::endl;

    return EXIT_SUCCESS;
}