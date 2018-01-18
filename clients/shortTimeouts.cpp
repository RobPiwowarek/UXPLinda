#include <iostream>
#include "../src/client/Client.h"

int main(void){
    Client client = Client();

    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec =0;
    srand (time(NULL));
    for (int i = 0; i < 10; ++i) {

        int num=rand()%10;
        timeout = {1, 0};
        std::string tuple = client.input(("Integer:=="+std::to_string(num)), &timeout);
        std::cout << "Received: " << tuple << std::endl;
    }
}