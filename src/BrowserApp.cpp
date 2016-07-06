#include "BrowserApp.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"
#include "Content.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

BrowserApp::BrowserApp()
{
}

void BrowserApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRegisterSchemeHandlerFactory("content", "",
		CefRefPtr<ContentHandlerFactory>(new ContentHandlerFactory()));

	CefRefPtr<BrowserHandler> handler(new BrowserHandler());

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;
	settings.file_access_from_file_urls = STATE_ENABLED;
	settings.universal_access_from_file_urls = STATE_ENABLED;
	settings.local_storage = STATE_DISABLED;

	CefRefPtr<CefBrowserView> browserView =
		CefBrowserView::CreateBrowserView(
			handler,
			"content:///index.html",
			settings,
			nullptr,
			nullptr
		);

	CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView));
}

void BrowserApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", true, false, false);
}
