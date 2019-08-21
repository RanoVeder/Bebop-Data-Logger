#pragma once
#include <librealsense2/rs.hpp>
#include <thread>
#include <atomic>
#include <memory>

#include "AppStates.h"
#include "Server.h"
#include "DataLogger.h"


class App
{
public:
    App();
    ~App();

    void Start();
    void Stop();

private:
    void InitRealsense();

private:
    //App modules
    std::shared_ptr<Server> server;
    std::shared_ptr<DataLogger> logger;

    //Logging threads (It would be better if these were spawned by Datalogger itself)
    std::thread image_logging_thread;
    std::thread data_logging_thread;
    std::thread server_thread;
    
    //Realsense 'state'
    std::shared_ptr<rs2::pipeline> pipe;

    std::atomic_bool running{true};
    AppStates appState;
};