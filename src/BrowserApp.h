#pragma once

#include <include/cef_app.h>

class BrowserApp :
	public CefApp,
	public CefBrowserProcessHandler
{
	IMPLEMENT_REFCOUNTING(BrowserApp);

public:
	BrowserApp();

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	virtual void OnContextInitialized() override;

	virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) override;

};
