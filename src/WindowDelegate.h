#pragma once

#include <include/views/cef_window.h>

class WindowDelegate : public CefWindowDelegate
{
	IMPLEMENT_REFCOUNTING(WindowDelegate);
	DISALLOW_COPY_AND_ASSIGN(WindowDelegate);

public:
	explicit WindowDelegate(CefRefPtr<CefBrowserView> browserView);

	void OnWindowCreated(CefRefPtr<CefWindow> window) override;
	void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
	bool CanClose(CefRefPtr<CefWindow> window) override;

private:
	CefRefPtr<CefBrowserView> m_browserView;
};
