#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {
    timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    sleep(4);
    Client client;

    std::cout << "Client ask10s input Integer:>1" << std::endl;
    std::string tuple = client.input("Integer:>1", &timeout);

    std::cout << "Client ask10s received tuple: " << tuple << std::endl;

}