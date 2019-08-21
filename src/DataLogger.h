#pragma once
#include <fstream>
#include <librealsense2/rs.hpp>
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>


#include "DataFrame.h"

class DataLogger
{
    public:
        DataLogger(std::shared_ptr<rs2::pipeline> pipe);
        ~DataLogger();

        void Stop();
        void Start();

        void Resume();
        void Pause();

        void queueDataFrame(DataFrame frame);

        void dataLogger();
        void imageLogger();

    private:
    std::shared_ptr<rs2::pipeline> rs2_pipe;
    std::queue<DataFrame> dataFrameQueue;
    std::mutex lock;

    int image_count = 0;

    std::atomic_bool threadRunning{true};
    std::atomic_bool running{false};
    std::atomic_bool paused{true};


    std::fstream left_image_file;
    std::fstream right_image_file;
    std::fstream data_logging_file;
};