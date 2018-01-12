#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include "Client.h"

#define SNAME "/mysem"

Client::Client() {
    pid = getpid();
    sem = sem_open(SNAME, 0);
}

Client::~Client() {

}

std::string Client::input(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::INPUT, pattern, timeout);
}

std::string Client::read(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::READ, pattern, timeout);
}


void Client::handleTimeout() {
    auto *cancellation = new Request(Request::RequestType::CANCEL, pid, nullptr, 0);
    write(WriteFD, cancellation, sizeof(cancellation));
    sem_post(sem);
}

std::string Client::handleSuccess() {
    char *data;
    int bytesRead = ::read(ReadFD, &data, 4);
    if (bytesRead < 4) {
        return "";
    }
    int dataSize = std::stoi(data);
    data = new char[dataSize];
    bytesRead = ::read(ReadFD, data, dataSize);
    if (bytesRead < dataSize) {
        return "";
    }
    return std::string(data);
    //TODO rozmiar, czy wiadomosc taka sama jak do serwera czy już z mniejszym nagłówkiem
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &pattern, timeval *timeout) {
    try {
        Pattern temp = Pattern(pattern);
    }catch(const std::invalid_argument &error){
        std::cout<<error.what();
        return "";
    }
    int patternSize=pattern.size();
    Request::RequestType type=Request::RequestType::OUTPUT;
    const char * patternChar= pattern.c_str();
    int msgSize=1+4+4+patternSize+1;
    char* str = new char[msgSize];
    memcpy(str , &type, 1);
    memcpy(str+1, &pid, 4);
    memcpy(str+5, &patternSize, 4);
    memcpy(str+9, patternChar, patternSize+1);

    write(WriteFD, str, sizeof(msgSize));
    sem_post(sem);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(ReadFD, &set);

    timeval timeout_select = *timeout; //select zeroes timeval!
    int value = select(ReadFD + 1, &set, nullptr, nullptr, &timeout_select); //ReadFD+1 couse select needs so

    if (value == -1) {//error
        return "";
    } else if (value == 0) {//timeOut
        handleTimeout();
        return "";
    } else {//success
        return handleSuccess();
    }
}

bool Client::output(std::string tuple) {
    try {
        Tuple temp = Tuple(tuple);
    }catch(const std::invalid_argument &error){
        std::cout<<error.what();
        return false;
    }
    int tupleSize=tuple.size();
    Request::RequestType type=Request::RequestType::OUTPUT;
    const char * tupleChar= tuple.c_str();
    int msgSize=1+4+4+tupleSize+1;
    char* str = new char[msgSize];
    memcpy(str, &type, 1);
    memcpy(str+1, &pid, 4);
    memcpy(str+5, &tupleSize, 4);
    memcpy(str+9, tupleChar, tupleSize+1);

    write(WriteFD, str, sizeof(msgSize));
    sem_post(sem);
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGKILL);
}
