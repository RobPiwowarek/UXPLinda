#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    Client client = Client();

    bool success = client.output("(5)");

    std::cout<< "Success in sending" <<success<<std::endl;

}