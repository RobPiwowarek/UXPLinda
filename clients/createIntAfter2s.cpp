#include <iostream>
#include <unistd.h>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {

    Client client;
    sleep(2);

    std::cout << "Client createIntAfter2s send tuple: " << "(10)" << std::endl;
    bool success = client.output("(10)");

    std::cout << "Client createIntAfter2s sent tuple (10) with success: " << success << std::endl;


}