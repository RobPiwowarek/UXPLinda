#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <climits>
#include "Client.h"

Client::Client() {
    pid = getpid();
}

Client::~Client() {
    close(ReadFD);
    close(WriteFD);
}

std::string Client::input(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::INPUT, pattern, timeout);
}

std::string Client::read(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::READ, pattern, timeout);
}


void Client::handleTimeout() {
//    auto *cancellation = new Request(Request::RequestType::CANCEL, pid, nullptr, 0);
//    sem_post(sem);
//    write(WriteFD, cancellation, sizeof(cancellation));
//    sem_post(sem);
}

std::string Client::handleSuccess() {
    char* data = new char[PIPE_BUF];
    std::string result;
    int bytesRead = ::read(ReadFD, data, PIPE_BUF);
    if(bytesRead < 1) {
        return "";
    }
    result = data;
    delete data;
    return result;
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &pattern, timeval *timeout) {
    try {
        Pattern temp = Pattern(pattern);
    } catch (const std::invalid_argument &error) {
        return "zly pattern";
    }
    int patternSize = pattern.size()+1;

    int msgSize = 4 + 4 + 4 + patternSize;
    char *str = new char[msgSize];
    memcpy(str, &requestType, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &patternSize, 4);
    memcpy(str + 12, pattern.c_str(), patternSize);
    write(WriteFD, str, msgSize);
    delete str;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(ReadFD, &set);

    timeval timeout_select = *timeout;
    int value = select(ReadFD + 1, &set, nullptr, nullptr, &timeout_select); //ReadFD+1 cause select needs so

    if (value == -1) {//error
        return "error";
    } else if (value == 0) {//timeOut
        handleTimeout();
        return "timeout";
    } else {//success
        return handleSuccess();
    }
}

bool Client::output(std::string tuple) {
    try {
        Tuple temp = Tuple(tuple);
    } catch (const std::invalid_argument &error) {
        std::cout << error.what();
        return false;
    }
    int tupleSize = tuple.size()+1;
    Request::RequestType type = Request::RequestType::OUTPUT;

    int msgSize = 4 + 4 + 4 + tupleSize;
    char *str = new char[msgSize];
    memcpy(str, &type, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &tupleSize, 4);
    memcpy(str + 12, tuple.c_str(), tupleSize);
    int success = write(WriteFD, str, msgSize);
    delete str;
    return success;
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGINT);
}
