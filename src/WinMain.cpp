#include "BrowserApp.h"
#include "RendererApp.h"

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

enum class AppType
{
	BROWSER,
	RENDERER,
	OTHER
};

const char *kProcessTypeSwitch = "type";
const char *kProcessType_Renderer = "renderer";

static AppType GetAppType(CefRefPtr<CefCommandLine> cmdLine)
{
	if (!cmdLine->HasSwitch(kProcessTypeSwitch))
		return AppType::BROWSER;

	auto processType = cmdLine->GetSwitchValue(kProcessTypeSwitch).ToString();

	if (processType == kProcessType_Renderer)
		return AppType::RENDERER;
	
	return AppType::OTHER;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	CefEnableHighDPISupport();

	CefMainArgs mainArgs(hInst);

	CefRefPtr<CefCommandLine> cmdLine = CefCommandLine::CreateCommandLine();
	cmdLine->InitFromString(::GetCommandLineW());

	CefRefPtr<CefApp> app;
	auto appType = GetAppType(cmdLine);
	if (appType == AppType::BROWSER)
		app = new BrowserApp();
	else if (appType == AppType::RENDERER)
		app = new RendererApp();

	int exitCode = CefExecuteProcess(mainArgs, app, nullptr);
	if (exitCode >= 0)
		return exitCode;

	CefSettings settings;
	settings.no_sandbox = true;
	settings.remote_debugging_port = 1337;
	//settings.single_process = true;

	CefInitialize(mainArgs, settings, app, nullptr);

	CefRunMessageLoop();

	CefShutdown();

	return 0;
}