#pragma once

#include "AppConfig.h"

#include <include/cef_client.h>
#include <vector>

class BrowserHandler :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefContextMenuHandler,
	public CefKeyboardHandler
{
	IMPLEMENT_REFCOUNTING(BrowserHandler);

public:
	explicit BrowserHandler(const AppConfig& config);
	
	~BrowserHandler();

	static BrowserHandler* Get();

	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }

	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }

	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;

	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;

	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event) override;

	void CloseAllBrowsers(bool forceClose);

	bool IsClosing() const { return m_isClosing; }

private:
	const AppConfig& m_config;
	bool m_isClosing;
	std::vector<CefRefPtr<CefBrowser>> m_browsers;
};

