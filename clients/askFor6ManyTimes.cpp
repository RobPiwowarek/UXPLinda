#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    Client client = Client();
    for (int i = 0; i < 5; i++) {
        timeout = {2, 0};
        std::cout << "asked for Integer:==6" << std::endl;
        std::string tuple = client.input("Integer:==6", &timeout);
        std::cout << "received tuple" << tuple << std::endl;
        if(tuple=="timeout" || tuple == "error"){
            std::cout << "ERROR Client ask10sFor6 hasn't received the tuple, make sure it was send" << tuple << std::endl;
        }
    }

}