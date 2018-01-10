#ifndef UXPLINDA_CLIENT_H
#define UXPLINDA_CLIENT_H

class Client {

public:
    static const int ReadFD = 1023;
    static const int WriteFD = 1022;

    void quit();
};


#endif //UXPLINDA_CLIENT_H
