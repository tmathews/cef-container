#include "WindowDelegate.h"

#include <include/views/cef_browser_view.h>

WindowDelegate::WindowDelegate(CefRefPtr<CefBrowserView> browserView)
	: m_browserView(browserView)
{
}

void WindowDelegate::OnWindowCreated(CefRefPtr<CefWindow> window)
{
	window->AddChildView(m_browserView);
	window->Show();

	m_browserView->RequestFocus();
}

void WindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window)
{
	m_browserView = nullptr;
}

bool WindowDelegate::CanClose(CefRefPtr<CefWindow> window)
{
	CefRefPtr<CefBrowser> browser = m_browserView->GetBrowser();
	if (browser)
		browser->GetHost()->TryCloseBrowser();
	return true;
}

