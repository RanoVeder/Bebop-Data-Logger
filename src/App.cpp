#include "App.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Logger.h"

App::App()
{
    InitRealsense();

    logger = std::make_shared<DataLogger>(pipe);
    server = std::make_shared<Server>(logger, 1337);

    appState = AppStates::DISCONNECTED;
}

App::~App()
{
    logger.reset();
    server.reset();
}


void App::Start()
{
    server_thread = std::thread(&Server::receiveData, server);
    image_logging_thread = std::thread(&DataLogger::imageLogger, logger);
    data_logging_thread = std::thread(&DataLogger::dataLogger, logger);

    logger->Start();
    server->Start();


    while(running)
    {
        appState = server->getState();
        switch(appState)
		{
			case AppStates::DISCONNECTED:
			break;

			case AppStates::STANDBY:
            logger->Pause();
			break;

			case AppStates::LOGGING:
            logger->Resume();
			break;
		}
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    server->Stop();
    logger->Stop();

    data_logging_thread.join();
    image_logging_thread.join();  
    server_thread.join();  
}

void App::Stop()
{
    running = false;  
}

void App::InitRealsense()
{   
    pipe = std::make_shared<rs2::pipeline>();
    rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_INFRARED, 1, 1280, 720, RS2_FORMAT_Y8, 30);
	cfg.enable_stream(RS2_STREAM_INFRARED, 2, 1280, 720, RS2_FORMAT_Y8, 30);
	rs2::pipeline_profile selection = pipe->start(cfg);
	rs2::device selected_device = selection.get_device();
	auto depth_sensor = selected_device.first<rs2::depth_sensor>();
	depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f);
}