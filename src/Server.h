#pragma once

#include <atomic>
#include <memory>

#include "AppStates.h"
#include "DataFrame.h"

#include "DataLogger.h"


class Server
{
    public:
        Server(std::shared_ptr<DataLogger> logger, int port);
        ~Server();

        void Start();
        void Stop();

        void receiveData();

        inline AppStates getState() const {return state;};

    private:
        std::atomic_bool threadRunning{true};
        std::atomic_bool running{false};
        std::shared_ptr<DataLogger> logger;

        int port;
        int socket_fd, client_fd;
        AppStates state = AppStates::DISCONNECTED;
};