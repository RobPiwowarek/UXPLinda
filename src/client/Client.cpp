#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include "Client.h"

#define SNAME "/mysem"

Client::Client() {
    pid = getpid();
    sem = sem_open(SNAME, 0);
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
    char *data;
    int bytesRead = ::read(ReadFD, data, 4);
    if (bytesRead < 4) {
        return std::to_string(bytesRead);
    }

    return std::to_string(bytesRead);
    int dataSize = std::stoi(data);
    data = new char[dataSize];
    bytesRead = ::read(ReadFD, data, dataSize);
    if (bytesRead < dataSize) {
        return "ano";
    }
    return std::string(data);
    //TODO rozmiar, czy wiadomosc taka sama jak do serwera czy już z mniejszym nagłówkiem
}

std::string Client::getTupleFromServer(Request::RequestType requestType, const std::string &pattern, timeval *timeout) {
    try {
        Pattern temp = Pattern(pattern);
    } catch (const std::invalid_argument &error) {
        return "zly pattern";
    }
    int patternSize = pattern.size();

    int msgSize = 4 + 4 + 4 + patternSize + 1;
    char *str = new char[msgSize];
    memcpy(str, &requestType, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &patternSize, 4);
    memcpy(str + 12, pattern.c_str(), patternSize);

    std::cout << pid<<" zapisano" << write(WriteFD, str, msgSize) << std::endl;
    sem_post(sem);

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
    int tupleSize = tuple.size();
    Request::RequestType type = Request::RequestType::OUTPUT;

    int msgSize = 4 + 4 + 4 + tupleSize + 1;
    char *str = new char[msgSize];
    memcpy(str, &type, 4);
    memcpy(str + 4, &pid, 4);
    memcpy(str + 8, &tupleSize, 4);
    memcpy(str + 12, tuple.c_str(), tupleSize + 1);
    int success = write(WriteFD, str, msgSize);
    std::cout << pid<< " wyslano" << success << std::endl;
    sem_post(sem);
    return success;
}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGKILL);
}
