#include <iostream>
#include "../src/client/Client.h"

int main(void){
    Client client = Client();

    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    for (int i = 0; i < 10; ++i) {
        client.output("(5, 10)");

        std::string tuple = client.input(("String: *"), &timeout);

        std::cout << "O-I: Got tuple: " << tuple << std::endl;
    }
}