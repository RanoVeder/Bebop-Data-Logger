#include "DataLogger.h"
#include "Logger.h"
#include <iostream>
#include <sys/stat.h>

#include <ctime>
#include <chrono>

DataLogger::DataLogger()
{

}

DataLogger::~DataLogger()
{

}

void DataLogger::init(rs2::pipeline *pipe)
{
    rs2_pipe = pipe;
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    char folderNameBuffer[19];
    std::strftime(folderNameBuffer, 19,"%Y_%m_%d__%H_%M", std::localtime(&now_time));

    std::string folderPath = "Bebop_Logs_" + std::string(folderNameBuffer);

    DEBUG_INFO("Folder created at: {}", folderPath);
    mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    data_logging_file = std::fstream(folderPath + "/data_logging_raw", std::ios::out | std::ios::binary);
    left_image_file = std::fstream(folderPath + "/data_left_camera_raw", std::ios::out | std::ios::binary);
    right_image_file = std::fstream(folderPath + "/data_right_camera_raw", std::ios::out | std::ios::binary);
}

void DataLogger::start()
{
    running = true;
}

void DataLogger::pause()
{
    paused = true;
}

void DataLogger::resume()
{
    paused = false;
}


void DataLogger::stop()
{
    DEBUG_INFO("Wrote {0} images", image_count);
    paused = true;
    running = false;
    threadRunning = false;
}

void DataLogger::queueDataFrame(DataFrame f)
{
    lock.lock();
    dataFrameQueue.push(f);
    lock.unlock();
}

void DataLogger::imageLogger()
{
    image_count = 0;

    while(threadRunning)
    {
        while(running)
        {
            while(!paused)
            {
                rs2::frameset frame = rs2_pipe->wait_for_frames();

                auto frameL = frame.get_infrared_frame(1);
                auto frameR = frame.get_infrared_frame(2);

                uint64_t fnum = frameL.get_frame_number();

                left_image_file.write((char*)&fnum, sizeof(uint64_t));
                left_image_file.write((char*)frameL.get_data(), 1280*720);

                right_image_file.write((char*)&fnum, sizeof(uint64_t));
                right_image_file.write((char*)frameR.get_data(), 1280*720);
                image_count++;
                DEBUG_INFO("Wrote {0} images", image_count);

            }
        }
    }
}


void DataLogger::dataLogger()
{
    int size = 0;
    DataFrame f;
    while(threadRunning)
    {
        while(running)
        {
            while(!paused)
            {
                lock.lock();
                // DEBUG_INFO("Queue size: {0} frames", size);
                if(dataFrameQueue.size() > 0)
                {
                    f = dataFrameQueue.front();
                    data_logging_file.write((char *)&f, sizeof(DataFrame));
                    dataFrameQueue.pop();
                }
                lock.unlock();
            }
        }
    }
}


