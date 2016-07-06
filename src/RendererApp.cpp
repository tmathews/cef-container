#include "RendererApp.h"
#include "Messages.h"
#include "AppConfig.h"
#include "Content.h"

inline bool ParseBoolString(const std::string &str)
{
	return str.compare("no") != 0
		&& str.compare("false") != 0
		&& str.compare("0") != 0;
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
		auto configMessage = CefProcessMessage::Create(kMsgConfigPayload);

		ParseHead(document->GetHead(), configMessage);

		m_browser->SendProcessMessage(PID_BROWSER, configMessage);
	}

	void ParseHead(CefRefPtr<CefDOMNode> head, CefProcessMessage *msg)
	{
		AppConfig config;

		for (auto child = head->GetFirstChild(); child; child = child->GetNextSibling())
		{
			auto elementTagName = child->GetElementTagName();
			if (elementTagName.compare("META") == 0
				&& child->HasElementAttribute("itemprop")
				&& child->HasElementAttribute("content")
				)
			{
				auto itemprop = child->GetElementAttribute("itemprop").ToString();
				
				auto cpos = itemprop.find_first_of(':');

				if (cpos == std::string::npos)
					continue; // Missing namespace

				auto cpos2 = itemprop.find_first_of(':', cpos + 1);

				if (cpos2 == std::string::npos)
					continue; // Missing section

				if ((cpos2 + 1) == itemprop.size())
					continue; // Missing key

				auto ns = itemprop.substr(0, cpos);
				
				if (ns.compare("app") != 0)
					continue; // Skip non-app namespaced itemprop

				auto section = itemprop.substr(cpos + 1, cpos2 - cpos - 1);
				auto key = itemprop.substr(cpos2 + 1);
				auto val = child->GetElementAttribute("content").ToString();

				if (section.compare("window") == 0)
				{
					if (key.compare("icon") == 0)
						strncpy(config.iconPath, val.c_str(), sizeof(config.iconPath));
				}
				else if (section.compare("fullscreen") == 0)
				{
					if (key.compare("enabled") == 0)
						config.fullscreenEnabled = ParseBoolString(val);
					else if (key.compare("key") == 0)
						strncpy(config.fullscreenKey, val.c_str(), sizeof(config.fullscreenKey));
					else if (key.compare("initial") == 0)
						config.fullscreenInitial = ParseBoolString(val);
					else if (key.compare("altenter") == 0)
						config.fullscreenAltEnter = ParseBoolString(val);
					else if (key.compare("f11") == 0)
						config.fullscreenF11 = ParseBoolString(val);
				}
				else if (section.compare("size") == 0)
				{
					if (key.compare("width") == 0)
						config.sizeWidth = std::stoi(val);
					else if (key.compare("height") == 0)
						config.sizeHeight = std::stoi(val);
				}
				else if (section.compare("minimize") == 0)
				{
					if (key.compare("enabled") == 0)
						config.minimizeEnabled = ParseBoolString(val);
				}
				else if (section.compare("maximize") == 0)
				{
					if (key.compare("maximize") == 0)
						config.maximizeEnabled = ParseBoolString(val);
				}
				else if (section.compare("resize") == 0)
				{
					if (key.compare("enabled") == 0)
						config.resizeEnabled = ParseBoolString(val);
				}
				else if (section.compare("contextmenu") == 0)
				{
					if (key.compare("enabled") == 0)
						config.contextmenuEnabled = ParseBoolString(val);
				}
				else if (section.compare("exit") == 0)
				{
					if (key.compare("esc") == 0)
						config.exitEsc = ParseBoolString(val);
				}
			}
		}

		msg->GetArgumentList()->SetBinary(kParamConfig,
			CefBinaryValue::Create(&config, sizeof(config)));
	}


private:
	CefRefPtr<CefBrowser> m_browser;
};

void RendererApp::OnWebKitInitialized()
{
	CefRegisterSchemeHandlerFactory("content", "",
		CefRefPtr<ContentHandlerFactory>(new ContentHandlerFactory()));
}

void RendererApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", true, true, false);
}

bool RendererApp::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message
)
{
	if (message->GetName().compare(kMsgIndexLoaded) == 0)
	{
		CefRefPtr<ConfigVisitor> visitor(new ConfigVisitor(browser));
		browser->GetMainFrame()->VisitDOM(visitor);
		return true;
	}
	return CefRenderProcessHandler::OnProcessMessageReceived(browser, source_process, message);
}
