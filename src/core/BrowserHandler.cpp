#include "BrowserHandler.h"
#include "AppConfig.h"
#include "IconDownloadCallback.h"
#include "App.h"

#include "platform/PlatformUtils.h"

#include <include/base/cef_bind.h>
#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/cef_app.h>
#include <include/cef_parser.h>

static BrowserHandler* s_instance;

BrowserHandler* BrowserHandler::Get()
{
	return s_instance;
}

BrowserHandler::BrowserHandler(App* app, const AppConfig& config)
	: m_app(app)
	, m_config(config)
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

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	m_browsers.push_back(browser);
}

bool BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_browsers.size() == 1)
	{
		m_isClosing = true;
	}

	return false;
}

void BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	auto it = std::find_if(m_browsers.begin(), m_browsers.end(), [browser](const CefRefPtr<CefBrowser> &other) {
		return other->IsSame(browser);
	});
	if (it != m_browsers.end())
	{
		m_browsers.erase(it);
	}

	if (m_browsers.empty())
	{
		CefQuitMessageLoop();
	}
}

void BrowserHandler::OnLoadError(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl
)
{
	CEF_REQUIRE_UI_THREAD();

	if (errorCode == ERR_ABORTED)
	{
		return;
	}

	std::stringstream ss;
	ss << "<html><body>"
		"Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</body></html>";
	return frame->LoadString(ss.str(), failedUrl);
};

void BrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	auto browserView = CefBrowserView::GetForBrowser(browser);
	auto browserWindow = browserView->GetWindow();

	browserWindow->SetFullscreen(m_config.fullscreenInitial);
	PlatformUtils::AdjustWindowSize(browserWindow, m_config.sizeWidth, m_config.sizeHeight);
	PlatformUtils::MakeWindowResizable(browserWindow, m_config.resizeEnabled);
	PlatformUtils::SetWindowAllowedActions(browserWindow, m_config.maximizeEnabled, m_config.minimizeEnabled);

	if (m_config.iconPath[0] != 0)
	{
		char iconUrl[128];

		CefURLParts urlParts;
		CefParseURL(m_config.iconPath, urlParts);

		if (urlParts.scheme.length == 0)
		{
			strcpy(iconUrl, "content://./");
			strcat(iconUrl, m_config.iconPath);
		}
		else
		{
			strncpy(iconUrl, m_config.iconPath, sizeof(iconUrl));
		}

		browser->GetHost()->DownloadImage(iconUrl, true, 16, true, new IconDownloadCallback(browser));
	}

	CefLoadHandler::OnLoadEnd(browser, frame, httpStatusCode);
}

void BrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	CEF_REQUIRE_UI_THREAD();

	if (!m_config.contextmenuEnabled)
	{
		model->Clear();
	}
}

bool BrowserHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
	if (event.type == KEYEVENT_KEYUP)
	{
		if (m_config.fullscreenEnabled)
		{
			if ((m_config.fullscreenF11 && event.windows_key_code == 0x7A)
				|| (m_config.fullscreenAltEnter && event.windows_key_code == 0x0D && (event.modifiers & EVENTFLAG_ALT_DOWN)))
			{
				auto browserView = CefBrowserView::GetForBrowser(browser);
				browserView->GetWindow()->SetFullscreen(!browserView->GetWindow()->IsFullscreen());
				return true;
			}
		}

		if (m_config.exitEsc && event.windows_key_code == 0x1B)
		{
			CloseAllBrowsers(false);
			return true;
		}
	}
	return false;
}

class CloseAllBrowsersTask : public CefTask
{
	IMPLEMENT_REFCOUNTING(CloseAllBrowsersTask);

public:
	CloseAllBrowsersTask(BrowserHandler* handler, bool forceClose)
		: m_handler(handler)
		, m_forceClose(forceClose)
	{
	}

	virtual void Execute() override
	{
		m_handler->CloseAllBrowsers(m_forceClose);
	}

private:
	BrowserHandler* m_handler;
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

	for (auto browser : m_browsers)
	{
		browser->GetHost()->CloseBrowser(forceClose);
	}
}

bool BrowserHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName() == "task_launch_container")
	{
		AppConfig config;
		message->GetArgumentList()->GetBinary(0)->GetData(&config, sizeof(AppConfig), 0);
		CefRefPtr<App::LaunchContainerTask> task(new App::LaunchContainerTask(m_app, config));
		return CefPostTask(TID_UI, task);
	}
	return false;
}

