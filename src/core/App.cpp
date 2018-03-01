#include "App.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"

#ifdef _WIN32
#include "content/io/WindowsArchive.h"
#else
#include "content/io/LinuxArchive.h"
#endif

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

static inline IArchive* NewPlatformArchive(const std::string& basePath)
{
#ifdef _WIN32
	return WindowsArchive::New(basePath);
#else
	return LinuxArchive::New(basePath);
#endif
}

App::App(const AppConfig& config)
	: m_config(config)
	, m_contentHandlerFactory(new ContentHandlerFactory())
{
	for (const auto& archivePath : m_config.archives)
	{
		m_contentHandlerFactory->MountArchive(NewPlatformArchive(archivePath));
	}
}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRegisterSchemeHandlerFactory("content", "", m_contentHandlerFactory);

	CefRefPtr<BrowserHandler> handler(new BrowserHandler(m_config));

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;
	settings.file_access_from_file_urls = STATE_ENABLED;
	settings.universal_access_from_file_urls = STATE_ENABLED;
	settings.local_storage = STATE_ENABLED;

	auto browserView = CefBrowserView::CreateBrowserView(
		handler,
		"content:///index.html",
		settings,
		nullptr,
		nullptr
	);

	CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView));
}

void App::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", true, false, false, false, false, false);
}
