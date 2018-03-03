#pragma once

#include "AppConfig.h"

#include "content/ContentHandlerFactory.h"

#include <include/cef_app.h>

struct AppConfig;

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
	IMPLEMENT_REFCOUNTING(App);

public:
	class LaunchContainerTask final : public CefTask
	{
		IMPLEMENT_REFCOUNTING(LaunchContainerTask);

	public:
		LaunchContainerTask(App* app, const AppConfig& config)
			: m_app(app)
			, m_config(config)
		{
		}

		void Execute() override
		{
			m_app->LaunchContainerInternal(m_config);
		}

	private:
		App* m_app;
		AppConfig m_config;
	};

public:
	explicit App(const AppConfig& config);

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

	virtual void OnContextInitialized() override;

	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

	virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

	bool LaunchContainer(CefRefPtr<CefBrowser> browser, const AppConfig& config);

private:
	bool LaunchContainerInternal(const AppConfig& config);

private:
	AppConfig m_config;
	CefRefPtr<ContentHandlerFactory> m_contentHandlerFactory;
};
