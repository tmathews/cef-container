#pragma once

#include <include/cef_browser.h>

class IconDownloadCallback final : public CefDownloadImageCallback
{
	IMPLEMENT_REFCOUNTING(IconDownloadCallback);

public:
	IconDownloadCallback(CefRefPtr<CefBrowser> browser);

	void OnDownloadImageFinished(const CefString& image_url, int http_status_code, CefRefPtr<CefImage> image) override;

private:
	CefRefPtr<CefBrowser> m_browser;
};
