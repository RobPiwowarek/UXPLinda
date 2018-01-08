#include "Request.h"

Request::~Request() {

}

int Request::getDataSize() const {
    return dataSize;
}

void Request::setDataSize(int dataSize) {
    Request::dataSize = dataSize;
}

pid_t Request::getClientID() const {
    return clientID;
}

char *Request::getData() const {
    return data;
}

void Request::setData(char *data) {
    Request::data = data;
}

Request::Request(Request::RequestType type, pid_t clientID, char *data, int dataSize): requestType(type), clientID(clientID), dataSize(dataSize) {
    this->data = data;
}

Request::RequestType Request::getRequestType() const {
    return requestType;
}
