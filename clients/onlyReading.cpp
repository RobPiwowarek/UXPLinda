#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    Client client;

    for(int i = 0; i < 3; ++i) {
        timeout = {1, 0};
        std::string t1 = client.read("Integer:10", &timeout);
        std::cout << "client onlyReading: received tuple: " << t1 << std::endl;
        sleep(2);
    }

}