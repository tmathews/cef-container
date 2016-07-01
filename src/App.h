#pragma once

#include <include/cef_app.h>

class App :
	public CefApp,
	public CefBrowserProcessHandler
{
	IMPLEMENT_REFCOUNTING(App);

public:
	App();

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
	{
		return this;
	}

	virtual void OnContextInitialized() override;
};
