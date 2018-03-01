#pragma once

#include <include/cef_app.h>

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
	IMPLEMENT_REFCOUNTING(App);

public:
	App();

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

	virtual void OnContextInitialized() override;

	virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
};
