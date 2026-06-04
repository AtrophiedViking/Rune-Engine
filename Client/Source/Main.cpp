#include "../Platform/Client/Application-Client.h"
#include "../Platform/Client/Window.h"
#include "Log/log.h"
#include "LauncherLayer.h"
int main()
{
	Rune::ApplicationConfig config;
	config.Name = "Rune Engine";
	config.windowConfig.Title = "Launcher";
	config.windowConfig.Width = 400;
	config.windowConfig.Height = 200;
	config.windowConfig.Fullscreen = false;
	config.windowConfig.Resizable = false;


	Rune::Application app(config);
	auto MainWindow = app.WindowCreate(config.windowConfig);
	MainWindow->PushLayer<LauncherLayer>();


	app.Run();
	return 0;
}