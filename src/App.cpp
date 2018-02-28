#include "App.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"
#include "Content.h"
#include "Messages.h"

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

App::App()
{
}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRegisterSchemeHandlerFactory("content", "", new ContentHandlerFactory());

	CefRefPtr<BrowserHandler> handler(new BrowserHandler());

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;
	settings.file_access_from_file_urls = STATE_ENABLED;
	settings.universal_access_from_file_urls = STATE_ENABLED;
	settings.local_storage = STATE_DISABLED;

	auto browserView = CefBrowserView::CreateBrowserView(
		handler,
		"content:///index.html",
		settings,
		nullptr,
		nullptr
	);

	CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView));
}

void App::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", true, false, false, false, false, false);
}

class ConfigVisitor : public CefDOMVisitor
{
	IMPLEMENT_REFCOUNTING(ConfigVisitor);

public:
	ConfigVisitor(CefRefPtr<CefBrowser> browser)
		: m_browser(browser)
	{
	}

	virtual void Visit(CefRefPtr<CefDOMDocument> document) override
	{
		auto configElement = document->GetElementById("config");

		rapidjson::Document configDoc;
		configDoc.Parse(configElement->GetElementInnerText().ToString().c_str());
		if (configDoc.HasParseError())
		{
#ifdef _WIN32
			OutputDebugStringA((std::string("Got parse error: ") + rapidjson::GetParseError_En(configDoc.GetParseError())).c_str());
#endif
			return;
		}

		auto windowObject = configDoc["window"].GetObject();

		AppConfig config;
		strcpy(config.iconPath, windowObject["icon"].GetString());

		config.sizeWidth = windowObject["width"].GetInt();
		config.sizeHeight = windowObject["height"].GetInt();
		config.resizeEnabled = windowObject["resizable"].GetBool();

		auto fullscreenObject = configDoc["fullscreen"].GetObject();
		config.fullscreenEnabled = fullscreenObject["enabled"].GetBool();
		config.fullscreenInitial = fullscreenObject["initial"].GetBool();
		config.fullscreenF11 = fullscreenObject["f11"].GetBool();
		config.fullscreenAltEnter = fullscreenObject["altEnter"].GetBool();

		config.contextmenuEnabled = configDoc["contextMenu"]["enabled"].GetBool();

		config.exitEsc = configDoc["exit"]["esc"].GetBool();

		auto configMessage = CefProcessMessage::Create(kMsgConfigPayload);
		configMessage->GetArgumentList()->SetBinary(kParamConfig, CefBinaryValue::Create(&config, sizeof(config)));
		m_browser->SendProcessMessage(PID_BROWSER, configMessage);
	}

private:
	CefRefPtr<CefBrowser> m_browser;
};

bool App::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName().compare(kMsgIndexLoaded) == 0)
	{
		CefRefPtr<ConfigVisitor> visitor(new ConfigVisitor(browser));
		browser->GetMainFrame()->VisitDOM(visitor);
		return true;
	}
	return CefRenderProcessHandler::OnProcessMessageReceived(browser, source_process, message);
}

