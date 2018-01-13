#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {

    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    Client client;

    std::cout << "Client timeoutAndInput is asking for " << "Integer:*" << std::endl;
    std::string tuple1 = client.input("Integer:*", &timeout);
    std::cout << "Client timeoutAndInput received: " << tuple1 << std::endl;

    std::cout << "Client timeoutAndInput is asking for: " << "String:*" << std::endl;
    std::string tuple2 = client.input("String:*", &timeout);
    std::cout << "Client timeoutAndInput received: " << tuple2 << std::endl;
}