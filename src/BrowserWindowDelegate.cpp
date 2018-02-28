#include "BrowserWindowDelegate.h"

#include <include/views/cef_browser_view.h>

BrowserWindowDelegate::BrowserWindowDelegate(CefRefPtr<CefBrowserView> browserView)
	: m_browserView(browserView)
{
}

void BrowserWindowDelegate::OnWindowCreated(CefRefPtr<CefWindow> window)
{
	window->AddChildView(m_browserView);
	window->Show();

	m_browserView->RequestFocus();
}

void BrowserWindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window)
{
	m_browserView = nullptr;
}

bool BrowserWindowDelegate::CanClose(CefRefPtr<CefWindow> window)
{
	auto browser = m_browserView->GetBrowser();
	if (browser)
	{
		browser->GetHost()->TryCloseBrowser();
	}
	return true;
}

