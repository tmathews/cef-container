#include "BrowserHandler.h"
#include "Messages.h"
#include "FileUtils.h"
#include "AppConfig.h"
#include "PlatformUtils.h"

#include <include/base/cef_bind.h>
#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/cef_app.h>

static BrowserHandler *s_instance;

BrowserHandler *BrowserHandler::Get()
{
	return s_instance;
}

BrowserHandler::BrowserHandler()
{
	s_instance = this;
}

BrowserHandler::~BrowserHandler()
{
	s_instance = nullptr;
}

void BrowserHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title)
{
	CEF_REQUIRE_UI_THREAD();

	auto browserView = CefBrowserView::GetForBrowser(browser);
	if (browserView)
	{
		auto window = browserView->GetWindow();
		if (window)
		{
			window->SetTitle(title);
		}
	}
}

class FaviconDownloadCallback : public CefDownloadImageCallback
{
	IMPLEMENT_REFCOUNTING(FaviconDownloadCallback);

public:
	FaviconDownloadCallback(CefRefPtr<CefBrowser> browser)
		: m_browser(browser)
	{
	}

	virtual void OnDownloadImageFinished(const CefString &image_url, int http_status_code, CefRefPtr<CefImage> image) override
	{
		if (!image)
			return;

		CEF_REQUIRE_UI_THREAD();

		auto browserView = CefBrowserView::GetForBrowser(m_browser);
		if (browserView)
		{
			auto window = browserView->GetWindow();
			if (window)
			{
				window->SetWindowIcon(image);
				window->SetWindowAppIcon(image);
			}
		}
	}

private:
	CefRefPtr<CefBrowser> m_browser;
};

void BrowserHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
{
	CEF_REQUIRE_UI_THREAD();

	if (!icon_urls.empty())
	{
		browser
			->GetHost()
			->DownloadImage(icon_urls[0], true, 16, false, new FaviconDownloadCallback(browser));
	}
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	m_browsers.push_back(browser);
}

bool BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_browsers.size() == 1)
		m_isClosing = true;

	return false;
}

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	auto it = std::find_if(m_browsers.begin(), m_browsers.end(),
		[browser](const CefRefPtr<CefBrowser> &other) { return other->IsSame(browser); });

	if (it != m_browsers.end())
		m_browsers.erase(it);

	if (m_browsers.empty())
		CefQuitMessageLoop();
}

void BrowserHandler::OnLoadError(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString &errorText,
	const CefString &failedUrl
)
{
	CEF_REQUIRE_UI_THREAD();

	if (errorCode == ERR_ABORTED)
		return;

	return frame->LoadString("<html>Something bad happened</html>", failedUrl);
};

void BrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	browser->SendProcessMessage(PID_RENDERER, CefProcessMessage::Create(kMsgIndexLoaded));
}

void BrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	CEF_REQUIRE_UI_THREAD();

	if (!m_config.contextmenuEnabled)
		model->Clear();
}

bool BrowserHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
	if (event.type == KEYEVENT_KEYUP)
	{
		if ((m_config.fullscreenF11 && event.windows_key_code == VK_F11)
		 || (m_config.fullscreenAltEnter && event.windows_key_code == VK_RETURN && (event.modifiers & EVENTFLAG_ALT_DOWN)))
		{
			auto browserView = CefBrowserView::GetForBrowser(browser);
			browserView->GetWindow()->SetFullscreen(!browserView->GetWindow()->IsFullscreen());
		}
		return true;
	}
	return false;
}

class CloseAllBrowsersTask : public CefTask
{
	IMPLEMENT_REFCOUNTING(CloseAllBrowsersTask);

public:
	CloseAllBrowsersTask(BrowserHandler *handler, bool forceClose)
		: m_handler(handler)
		, m_forceClose(forceClose)
	{
	}

	virtual void Execute() override
	{
		m_handler->CloseAllBrowsers(m_forceClose);
	}

private:
	BrowserHandler *m_handler;
	bool m_forceClose;
};

void BrowserHandler::CloseAllBrowsers(bool forceClose)
{
	if (!CefCurrentlyOn(TID_UI))
	{
		CefRefPtr<CloseAllBrowsersTask> task(new CloseAllBrowsersTask(this, forceClose));
		CefPostTask(TID_UI, task);
		return;
	}

	if (m_browsers.empty())
		return;

	for (auto browser : m_browsers)
		browser->GetHost()->CloseBrowser(forceClose);
}

bool BrowserHandler::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message
)
{
	if (message->GetName().compare(kMsgConfigPayload) == 0)
	{
		auto browserView = CefBrowserView::GetForBrowser(browser);
		auto browserWindow = browserView->GetWindow();

		auto configBinary = message->GetArgumentList()->GetBinary(kParamConfig);
		if (configBinary)
		{
			size_t got = configBinary->GetData(&m_config, sizeof(m_config), 0);
			if (got == sizeof(m_config))
			{
				browserWindow->SetFullscreen(m_config.fullscreenInitial);
				browserWindow->SetSize(CefSize(m_config.sizeWidth, m_config.sizeHeight));
				PlatformUtils::MakeWindowResizable(browserWindow, m_config.resizeEnabled);
				PlatformUtils::MakeWindowMaximizable(browserWindow, m_config.maximizeEnabled);
				PlatformUtils::MakeWindowMinimizable(browserWindow, m_config.minimizeEnabled);
			}
		}

		return true;
	}
	return CefClient::OnProcessMessageReceived(browser, source_process, message);
}
