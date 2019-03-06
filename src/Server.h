#pragma once
#include "AppStates.h"
#include "DataFrame.h"

#include "DataLogger.h"

class Server
{
    public:
        Server(int port);

        void init(DataLogger *logger);
        void start();
        void stop();

        void receiveData();

        inline AppStates getState() const {return state;};

    private:
        bool threadRunning = true;
        bool running = false;
        DataLogger *logger;

        int port;
        int socket_fd, client_fd;
        AppStates state = AppStates::DISCONNECTED;
};