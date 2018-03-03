#include "JSContainer.h"

#include "core/AppConfig.h"
#include "core/App.h"

static const char* kMissingConfigException = "Config file was not specified.";

static const char* kConfigLoadingError = "There was an error loading the configuration file.";

JSContainer::NewHandler::NewHandler(App* app, CefRefPtr<CefBrowser> browser)
	: m_app(app)
	, m_browser(browser)
{
}

bool JSContainer::NewHandler::Execute(
	const CefString& name,
	CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception
)
{
	if (arguments.empty() || !arguments[0]->IsString())
	{
		exception = kMissingConfigException;
		return false;
	}

	AppConfig config;
	if (!LoadAppConfig(arguments[0]->GetStringValue().ToString().c_str(), &config))
	{
		exception = kConfigLoadingError;
		return false;
	}

	return m_app->LaunchContainer(m_browser, config);
}
