#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main ( int argc, char *argv[] )
{
    timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    Client client = Client();

    std::cout << "Client rand output (5)" << std::endl;
    bool success = client.output("(5)");

    std::cout << "Client rand's creating random tuples" << std::endl;
    srand (time(NULL));
    for(int i = 0 ; i<7000; i++){
        timeout = {20, 0};
        int num=rand()%10;
        std::string msg="("+std::to_string(num)+")";
        client.output(msg);
    }

}