#pragma once

#include <include/cef_app.h>

class RendererApp :
	public CefApp,
	public CefRenderProcessHandler
{
	IMPLEMENT_REFCOUNTING(RendererApp);

public:
	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process, 
		CefRefPtr<CefProcessMessage> message
	) override;

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
};

inline CefRefPtr<CefRenderProcessHandler> RendererApp::GetRenderProcessHandler()
{
	return this;
}
