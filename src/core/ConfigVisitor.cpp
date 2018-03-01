#include "ConfigVisitor.h"
#include "AppConfig.h"
#include "Messages.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

ConfigVisitor::ConfigVisitor(CefRefPtr<CefBrowser> browser)
	: m_browser(browser)
{
}

void ConfigVisitor::Visit(CefRefPtr<CefDOMDocument> document)
{
	auto configElement = document->GetElementById("config");

	rapidjson::Document configDoc;
	configDoc.Parse(configElement->GetElementInnerText().ToString().c_str());
	if (configDoc.HasParseError())
	{
#ifdef _WIN32
		// TODO: Improve error reporting.
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

	auto configMessage = CefProcessMessage::Create(kMessageConfigure);
	configMessage->GetArgumentList()->SetBinary(kParamConfig, CefBinaryValue::Create(&config, sizeof(config)));
	m_browser->SendProcessMessage(PID_BROWSER, configMessage);
}
