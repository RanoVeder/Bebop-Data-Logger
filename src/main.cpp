#include <librealsense2/rs.hpp>
#include <iostream>

int main()
{
	rs2::context ctx;
	auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
	if (list.size() == 0)
		throw std::runtime_error("No device detected. Is it plugged in?");
	
	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_INFRARED, 0);
	cfg.enable_stream(RS2_STREAM_INFRARED, 1);

	rs2::pipeline pipe;
	pipe.start();


	for (int i = 0; i < 30; i++) pipe.wait_for_frames();;
	while (true)
	{
		auto frame = pipe.wait_for_frames();
		auto a = frame.get_color_frame().get_bits_per_pixel();
		//auto d = a.get_profile().stream_type();
		std::cout << a << std::endl;

	}

	return 0;
}
