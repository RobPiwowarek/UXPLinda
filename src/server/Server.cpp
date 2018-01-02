#include "Server.h"

Server::Server(std::list<char*> fileNames) {
    tupleSpace = new TupleSpace();

    this->fileNames = fileNames;

    RESULT_PIPE_FDS = new int*[fileNames.size()];

    for (int i = 0; i < fileNames.size(); ++i){
        RESULT_PIPE_FDS[i] = new int[2];
    }
}

Server::~Server() {
    delete tupleSpace;
}

int Server::run() {
    int tempFD[2];

    if (pipe(tempFD) < 0) // tempFD[0] wejsciowy dla serwera, z tego odczytuje requesty od klientow
        return(NULL); // error -> brak wolnych deskryptorow

    for (int i = 0; i < fileNames.size(); ++i){
        int clientFD[2];

        if (pipe(clientFD) < 0)
            return(NULL); // error -> brak wolnych deskryptorow

        RESULT_PIPE_FDS[i][0] = clientFD[0];
        RESULT_PIPE_FDS[i][1] = clientFD[1];
    }

    int pipeIndex = 0;
    for (auto file: fileNames){
        pid_t pid = fork();

        if (pid == 0){
            // tempFD[1] wyjsciowy dla requestow, dla klientow, ten sam dla kazdego klienta -> tutaj pisza requesty
            // otworz/zamknij odpowiedni pipe
            close(tempFD[0]);
            close(RESULT_PIPE_FDS[pipeIndex][1]);
            execve(file, nullptr, nullptr); // TODO: pewnie trzeba bedzie przekazac deskryptory potokow - a przynajmniej index, also chyba lepiej uzyc execl
        }
        else {
            childrenPIDs.push_back(pid);
        }

        ++pipeIndex;
    }

    // closing READ pipes for parent
    close(tempFD[1]);

    for(int i = 0; i < fileNames.size(); ++i){
        close(RESULT_PIPE_FDS[i][0]);
    }

    return 0;
}
