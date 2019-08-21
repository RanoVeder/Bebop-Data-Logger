#include <iostream>
#include <signal.h>
#include <memory>

#include "App.h"
#include "Logger.h"

std::shared_ptr<App> app;

void onClose(int s)
{
	app->Stop();
}

int main()
{
	Logger::Init();
	app = std::make_shared<App>();
	
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = onClose;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	app->Start();

	//Force app deallocation before the logging singleton deallocates :)
	app.reset();
	return 0;
}
