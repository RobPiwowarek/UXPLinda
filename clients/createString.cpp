#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    Client client;

    std::cout << "Client rand output (abc)" << std::endl;
    bool success = client.output("(\"abc\")");
}
