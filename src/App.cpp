#include "App.h"
#include <iostream>
#include "Logger.h"

App::App(): server(new Server(1337)), logger(new DataLogger())
    {
    server_thread = std::thread(&Server::receiveData, server);
    image_logging_thread = std::thread(&DataLogger::imageLogger, logger);
    data_logging_thread = std::thread(&DataLogger::dataLogger, logger);
}

App::~App()
{

}

void App::init()
{
    initRealsense();

    server->init(logger);
    logger->init(&pipe);

    appState = AppStates::DISCONNECTED;

}

void App::start()
{
    server->start();
    logger->start();


    while(running)
    {
        appState = server->getState();
        switch(appState)
		{
			case AppStates::DISCONNECTED:
			break;

			case AppStates::STANDBY:
            logger->pause();
			break;

			case AppStates::LOGGING:
            logger->resume();
			break;
		}
    }

}

void App::stop()
{
    void * status;
    server->stop();
    logger->stop();

    // DEBUG_INFO("Server exited with status: {}", status);

    // DEBUG_INFO("Data logging exited with status: {}", status);
    server_thread.join();    

    // image_logging_thread.join();
    data_logging_thread.join();
}

void App::initRealsense()
{
    rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_INFRARED, 1, 1280, 720, RS2_FORMAT_Y8, 30);
	cfg.enable_stream(RS2_STREAM_INFRARED, 2, 1280, 720, RS2_FORMAT_Y8, 30);
	rs2::pipeline_profile selection = pipe.start(cfg);
	rs2::device selected_device = selection.get_device();
	auto depth_sensor = selected_device.first<rs2::depth_sensor>();
	depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f);
}