#include "App.h"
#include "Handler.h"
#include "WindowDelegate.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

App::App()
{
}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<Handler> handler(new Handler());

	CefBrowserSettings settings;
	settings.web_security = STATE_ENABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;

	CefRefPtr<CefBrowserView> browserView =
		CefBrowserView::CreateBrowserView(
			handler,
			"http://www.google.com/",
			settings,
			nullptr,
			nullptr
		);

	CefWindow::CreateTopLevelWindow(new WindowDelegate(browserView));
}
