#pragma once
#include "AppStates.h"

#include <librealsense2/rs.hpp>
#include <thread>

#include "Server.h"
#include "DataLogger.h"


class App
{
public:
    App();
    ~App();

    void init();
    void start();
    void stop();
private:
    void initRealsense();

private:
rs2::pipeline pipe;

Server *server;
DataLogger *logger;

bool running = true;

std::thread image_logging_thread;
std::thread data_logging_thread;
std::thread server_thread;

AppStates appState;

};