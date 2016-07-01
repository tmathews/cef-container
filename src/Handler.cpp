#include "Handler.h"

#include <include/base/cef_bind.h>
#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/cef_app.h>

static Handler *s_instance;

Handler *Handler::Get()
{
	return s_instance;
}

Handler::Handler()
{
	s_instance = this;
}

Handler::~Handler()
{
	s_instance = nullptr;
}

void Handler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title)
{
	CEF_REQUIRE_UI_THREAD();

#ifdef _WIN32
	auto browserView = CefBrowserView::GetForBrowser(browser);
	if (browserView)
	{
		auto window = browserView->GetWindow();
		if (window)
		{
			auto hWnd = window->GetWindowHandle();
			SetWindowTextW(hWnd, title.c_str());
		}
	}
#endif
}

void Handler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	m_browsers.push_back(browser);
}

bool Handler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_browsers.size() == 1)
		m_isClosing = true;

	return false;
}

void Handler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	auto it = std::find_if(m_browsers.begin(), m_browsers.end(),
		[browser](const CefRefPtr<CefBrowser> &other) { return other->IsSame(browser); });

	if (it != m_browsers.end())
		m_browsers.erase(it);

	if (m_browsers.empty())
		CefQuitMessageLoop();
}

void Handler::OnLoadError(
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

class CloseAllBrowsersTask : public CefTask
{
	IMPLEMENT_REFCOUNTING(CloseAllBrowsersTask);

public:
	CloseAllBrowsersTask(Handler *handler, bool forceClose)
		: m_handler(handler)
		, m_forceClose(forceClose)
	{
	}

	virtual void Execute() override
	{
		m_handler->CloseAllBrowsers(m_forceClose);
	}

private:
	Handler *m_handler;
	bool m_forceClose;
};

void Handler::CloseAllBrowsers(bool forceClose)
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
