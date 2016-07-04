#include "BrowserApp.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"
#include "FileUtils.h"
#include "Content.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

BrowserApp::BrowserApp()
{
}

void BrowserApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<BrowserHandler> handler(new BrowserHandler());

	CefRegisterSchemeHandlerFactory("content", "", ContentHandlerFactory::Get());

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;

	CefRefPtr<CefBrowserView> browserView =
		CefBrowserView::CreateBrowserView(
			handler,
			"content://index.html",
			settings,
			nullptr,
			nullptr
		);

	CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView));
}

void BrowserApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", false, false, false);
}
