#pragma once

#include <include/cef_app.h>

class RendererApp :
	public CefApp,
	public CefRenderProcessHandler
{
	IMPLEMENT_REFCOUNTING(RendererApp);

public:
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process, 
		CefRefPtr<CefProcessMessage> message
	) override;

	virtual void OnWebKitInitialized() override;

	virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) override;
};
