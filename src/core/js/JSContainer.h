#pragma once

#include <include/cef_v8.h>
#include <include/cef_browser.h>

class App;

namespace JSContainer
{
	class NewHandler final : public CefV8Handler
	{
		IMPLEMENT_REFCOUNTING(NewHandler);

	public:
		explicit NewHandler(App* app, CefRefPtr<CefBrowser> browser);

		bool Execute(
			const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception
		) override;

	private:
		App* m_app;
		CefRefPtr<CefBrowser> m_browser;
	};
};

