#include <librealsense2/rs.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <signal.h>

#include <chrono>



using namespace std::chrono;

const int width = 1280;
const int height = 720;
const char* folder = "data/";

bool running = true;
uint64_t count = 0;

const unsigned int Capacity = 5;
rs2::frame_queue queueL(Capacity);
rs2::frame_queue queueR(Capacity);

std::fstream left_file;
std::fstream right_file;

std::thread write_thread;





void onClose(int s)
{
	std::cout << "catched exit signal: " << s << std::endl;
	running=false; 
}

void imageLogging()
{
	while(running)
	{
		rs2::video_frame frameL = queueL.wait_for_frame();	
		rs2::video_frame frameR = queueR.wait_for_frame();

		int64_t time = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();

		left_file.write((char*)&time, sizeof(int64_t));
		left_file.write((char*)frameL.get_data(), width*height);

		right_file.write((char*)&time, sizeof(int64_t));
		right_file.write((char*)frameR.get_data(), width*height);
		count++;
	}
}


int main()
{

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = onClose;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);


	rs2::context ctx;
	auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
	if (list.size() == 0)
		throw std::runtime_error("No device detected. Is it plugged in?");

	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_INFRARED, 1, width, height, RS2_FORMAT_Y8, 30);
	cfg.enable_stream(RS2_STREAM_INFRARED, 2, width, height, RS2_FORMAT_Y8, 30);
	rs2::pipeline pipe;
	pipe.start(cfg);


	left_file = std::fstream("file_left", std::ios::out | std::ios::binary);
	right_file = std::fstream("file_right", std::ios::out | std::ios::binary);

	write_thread = std::thread(imageLogging);
	while (running)
	{
		rs2::frameset frame = pipe.wait_for_frames();

		auto frameL = frame.get_infrared_frame(1);
		queueL.enqueue(frameL);
		auto frameR = frame.get_infrared_frame(2);
		queueR.enqueue(frameR);
	}
	std::cout << "waiting for logging thread to close..." << std::endl;
	write_thread.join();
	std::cout << "logging thread closed. " << count << " images logged."<<  std::endl;
	left_file.close();
	right_file.close();


	return 0;
}
