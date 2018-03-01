#include "core/App.h"
#include "core/AppConfig.h"

static const char* CACHE_PATH = "./cache";

int main(int argc, char* argv[])
{
	CefEnableHighDPISupport();

	CefMainArgs mainArgs(argc, argv);

	auto cmdLine = CefCommandLine::CreateCommandLine();
	cmdLine->InitFromArgv(argc, argv);

	std::string configPath = cmdLine->GetSwitchValue("config").ToString();
	if (configPath.empty())
	{
		configPath = "config.default.json";
	}

	AppConfig config;
	if (!LoadAppConfig(configPath.c_str(), &config))
	{
		return -1;
	}

	CefRefPtr<CefApp> app(new App(config));

	int exitCode = CefExecuteProcess(mainArgs, app, nullptr);
	if (exitCode >= 0)
	{
		return exitCode;
	}

	CefSettings settings;
	settings.no_sandbox = true;
	settings.remote_debugging_port = 1337;
	settings.single_process = false;
	CefString(&settings.cache_path).FromASCII(CACHE_PATH);

	CefInitialize(mainArgs, settings, app, nullptr);

	CefRunMessageLoop();

	CefShutdown();

	return 0;
}
