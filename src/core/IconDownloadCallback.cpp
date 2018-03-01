#include "IconDownloadCallback.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>

IconDownloadCallback::IconDownloadCallback(CefRefPtr<CefBrowser> browser)
	: m_browser(browser)
{
}

void IconDownloadCallback::OnDownloadImageFinished(const CefString& image_url, int http_status_code, CefRefPtr<CefImage> image)
{
	if (!image)
	{
		return;
	}

	CEF_REQUIRE_UI_THREAD();

	auto browserView = CefBrowserView::GetForBrowser(m_browser);
	if (!browserView)
	{
		return;
	}

	auto window = browserView->GetWindow();
	if (window)
	{
		window->SetWindowIcon(image);
		window->SetWindowAppIcon(image);
	}
}
