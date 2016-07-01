#include "App.h"

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	CefEnableHighDPISupport();

	CefMainArgs mainArgs(hInst);

	int exitCode = CefExecuteProcess(mainArgs, nullptr, nullptr);
	if (exitCode >= 0)
		return exitCode;

	CefSettings settings;
	settings.no_sandbox = true;

	CefRefPtr<App> app(new App());

	CefInitialize(mainArgs, settings, app, nullptr);

	CefRunMessageLoop();

	CefShutdown();

	return 0;
}