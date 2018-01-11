#include <ctime>
#include <string>     // std::string, std::to_string
//#include <zconf.h>
#include <signal.h>
#include <iostream>
#include "Client.h"


Client::Client() {
    pid = getpid();
    sem = sem_open(SNAME, O_CREAT, 0644, 3);
}

Client::~Client() {

}

std::string Client::input(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::INPUT, pattern, timeout);
}

std::string Client::read(const std::string &pattern, timeval *timeout) {
    return getTupleFromServer(Request::RequestType::READ, pattern, timeout);
}

char *getWritable(const std::string &str) {
    auto *writable = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0';
    return writable;
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
    std::string str;
    str += std::to_string(static_cast<int>(requestType));
    str += std::to_string(pid);
    str += std::to_string(pattern.size());
    str += pattern;

    write(WriteFD, str.c_str(), str.size()+1);//?+1?
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

bool Client::output(const std::string &tuple) {
    try {
        Tuple temp = Tuple(tuple);
    }catch(const std::invalid_argument &error){
        std::cout<<error.what();
        return false;
    }

    std::string str;
    str += std::to_string(static_cast<int>(Request::RequestType::OUTPUT));
    str += std::to_string(pid);
    str += std::to_string(tuple.size());
    str += tuple;
    write(WriteFD, str.c_str(), str.size()+1);

}

void Client::quit() {
    pid_t ppid = getppid();

    kill(ppid, SIGKILL);
}
