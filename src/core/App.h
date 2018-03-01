#pragma once

#include <include/cef_app.h>

class ContentHandlerFactory;
struct AppConfig;

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
	IMPLEMENT_REFCOUNTING(App);

public:
	explicit App(const AppConfig& config);

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

	virtual void OnContextInitialized() override;

	virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

private:
	const AppConfig& m_config;
	CefRefPtr<ContentHandlerFactory> m_contentHandlerFactory;
};
