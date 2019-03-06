#pragma once
#include <fstream>
#include <librealsense2/rs.hpp>
#include <queue>
#include <mutex>


#include "DataFrame.h"

class DataLogger
{
    public:
        DataLogger();
        ~DataLogger();

        void init(rs2::pipeline *pipe);
        void stop();
        void start();

        void resume();
        void pause();

        void queueDataFrame(DataFrame frame);

        void dataLogger();
        void imageLogger();

    private:
    rs2::pipeline *rs2_pipe;
    std::queue<DataFrame> dataFrameQueue;
    std::mutex lock;

    int image_count = 0;

    bool threadRunning = true;
    bool running = false;
    bool paused = true;

    std::fstream left_image_file;
    std::fstream right_image_file;
    std::fstream data_logging_file;
};