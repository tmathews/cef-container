#include "App.h"
#include "Content.h"

static const char* CACHE_PATH = "./cache";

int main(int argc, char* argv[])
{
	CefEnableHighDPISupport();

	CefMainArgs mainArgs(argc, argv);

	CefRefPtr<CefApp> app = new App();

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
