#pragma once

#include <include/views/cef_window.h>

class BrowserWindowDelegate : public CefWindowDelegate
{
	IMPLEMENT_REFCOUNTING(BrowserWindowDelegate);
	DISALLOW_COPY_AND_ASSIGN(BrowserWindowDelegate);

public:
	explicit BrowserWindowDelegate(CefRefPtr<CefBrowserView> browserView);

	void OnWindowCreated(CefRefPtr<CefWindow> window) override;
	void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
	bool CanClose(CefRefPtr<CefWindow> window) override;

private:
	CefRefPtr<CefBrowserView> m_browserView;
};
