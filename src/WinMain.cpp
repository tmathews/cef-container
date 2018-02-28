#include "App.h"
#include "Content.h"

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

static const char* CACHE_PATH = "./cache";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	CefEnableHighDPISupport();

	CefMainArgs mainArgs(hInst);

	auto cmdLine = CefCommandLine::CreateCommandLine();
	cmdLine->InitFromString(::GetCommandLineW());

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