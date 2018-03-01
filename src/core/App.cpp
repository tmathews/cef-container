#include "App.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"
#include "Messages.h"
#include "ConfigVisitor.h"

#include "content/ContentHandlerFactory.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

App::App()
{
}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRegisterSchemeHandlerFactory("content", "", new ContentHandlerFactory());

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

bool App::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName().compare(kMessageVisitIndex) == 0)
	{
		CefRefPtr<ConfigVisitor> visitor(new ConfigVisitor(browser));
		browser->GetMainFrame()->VisitDOM(visitor);
		return true;
	}
	return CefRenderProcessHandler::OnProcessMessageReceived(browser, source_process, message);
}

