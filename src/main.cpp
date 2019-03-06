#include "App.h"

#include <iostream>
#include <signal.h>
#include "Logger.h"



App *app;

void onClose(int s)
{
	DEBUG_INFO("Catched exit signal: {0}", s);
	app->stop();

}

int main()
{
	Logger::Init();
	app = new App();
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = onClose;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	app->init();
	app->start();

	return 0;
}
