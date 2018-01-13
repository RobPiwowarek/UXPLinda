#include <iostream>
#include <zconf.h>
#include <string>
#include "../src/client/Client.h"

int main(int argc, char *argv[]) {
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    Client client;

    while(true){
	int a = 0;
        std::cout << "Podaj operacje: " << std::endl;
        std::cout << "0 - input" << std::endl;
		std::cout << "1 - output" << std::endl;
        std::cout << "2 - read" << std::endl;
		std::cout << "3 - quit" << std::endl;
        std::cin >> a;

	std::string tuple;

	switch (a) {
		case 0: 
			std::cout << "Podaj pattern\n" << std::endl;
            getchar();
			getline(std::cin, tuple);
			tuple = client.input(tuple, &timeout);
			std::cout << "Tuple: " << tuple << std::endl;
			break;
		case 1: 
			std::cout << "Podaj krotke\n" << std::endl;
            getchar();
            getline(std::cin, tuple);
            client.output(tuple);
			break;
		case 2: 
			std::cout << "Podaj pattern\n" << std::endl;
            getchar();
			getline(std::cin, tuple);
			tuple = client.read(tuple, &timeout);
			std::cout << "Tuple: " << tuple << std::endl;
			break;
		case 3: 
			client.quit();
			break;
		default:
			std::cout << "jestes uposledzony" << std::endl;
	}

    }

}
