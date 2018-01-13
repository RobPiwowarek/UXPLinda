#include <iostream>
//#include <zconf.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    Client client;

    for(int i = 0; i < 10; ++i) {
        std::string t1 = client.input("Integer:*, Integer:*", &timeout);
        std::cout << "client inputOutput: received tuple: " << t1 << std::endl;
        client.output("(\":o)\")");
    }

}