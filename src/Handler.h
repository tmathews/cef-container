#pragma once

#include <include/cef_client.h>
#include <vector>

class Handler:
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler
{
	IMPLEMENT_REFCOUNTING(Handler);

public:
	Handler();
	~Handler();

	static Handler *Get();

	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title) override;

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	virtual void OnLoadError(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString &errorText,
		const CefString &failedUrl
	) override;

	void CloseAllBrowsers(bool forceClose);

	bool IsClosing() const { return m_isClosing; }

private:
	bool m_isClosing;
	std::vector<CefRefPtr<CefBrowser>> m_browsers;
};

