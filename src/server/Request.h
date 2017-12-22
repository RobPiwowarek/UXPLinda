#ifndef UXPLINDA_REQUEST_H
#define UXPLINDA_REQUEST_H

class Request {
public:

    enum class RequestType {
        OUTPUT,
        READ,
        INPUT
    };

    Request(RequestType type, char *clientID, char &data);

    virtual ~Request();

private:
    int dataSize;

    char clientID[2];

    char *data;
};



#endif //UXPLINDA_REQUEST_H
