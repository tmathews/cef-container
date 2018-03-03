#include "App.h"
#include "BrowserHandler.h"
#include "BrowserWindowDelegate.h"

#include "js/JSContainer.h"

#ifdef _WIN32
#include "content/io/WindowsArchive.h"
#else
#include "content/io/LinuxArchive.h"
#endif

#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>

static inline IArchive* NewPlatformArchive(const std::string& basePath)
{
#ifdef _WIN32
	return WindowsArchive::New(basePath);
#else
	return LinuxArchive::New(basePath);
#endif
}

App::App(const AppConfig& config)
	: m_config(config)
	, m_contentHandlerFactory(new ContentHandlerFactory())
{
	for (const auto& archivePath : m_config.archives)
	{
		m_contentHandlerFactory->MountArchive(NewPlatformArchive(archivePath));
	}
}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefRegisterSchemeHandlerFactory("content", "", m_contentHandlerFactory);

	CefRefPtr<BrowserHandler> handler(new BrowserHandler(this, m_config));

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;
	settings.file_access_from_file_urls = STATE_ENABLED;
	settings.universal_access_from_file_urls = STATE_ENABLED;
	settings.local_storage = STATE_ENABLED;

	auto browserView = CefBrowserView::CreateBrowserView(
		handler,
		"content:///index.html",
		settings,
		nullptr,
		nullptr
	);

	CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView));
}

void App::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	CefRefPtr<CefV8Value> global = context->GetGlobal();
	
	auto containerObject = CefV8Value::CreateObject(nullptr, nullptr);
	containerObject->SetValue(
		"new",
		CefV8Value::CreateFunction("new", CefRefPtr<CefV8Handler>(new JSContainer::NewHandler(this, browser))),
		V8_PROPERTY_ATTRIBUTE_READONLY
	);
	global->SetValue("container", containerObject, V8_PROPERTY_ATTRIBUTE_READONLY);
}

void App::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
	registrar->AddCustomScheme("content", true, false, false, false, false, false);
}

bool App::LaunchContainer(CefRefPtr<CefBrowser> browser, const AppConfig& config)
{
	CefRefPtr<CefBinaryValue> packagedConfig(CefBinaryValue::Create(&config, sizeof(config)));

	CefRefPtr<CefProcessMessage> message(CefProcessMessage::Create("task_launch_container"));
	message->GetArgumentList()->SetBinary(0, packagedConfig);
	return browser->SendProcessMessage(PID_BROWSER, message);
}

bool App::LaunchContainerInternal(const AppConfig& config)
{
	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<BrowserHandler> handler(new BrowserHandler(this, config));

	CefBrowserSettings settings;
	settings.web_security = STATE_DISABLED;
	settings.remote_fonts = STATE_ENABLED;
	settings.image_loading = STATE_ENABLED;
	settings.webgl = STATE_ENABLED;
	settings.plugins = STATE_ENABLED;
	settings.file_access_from_file_urls = STATE_ENABLED;
	settings.universal_access_from_file_urls = STATE_ENABLED;
	settings.local_storage = STATE_ENABLED;

	auto browserView = CefBrowserView::CreateBrowserView(
		handler,
		"content:///index.html",
		settings,
		nullptr,
		nullptr
	);

	return CefWindow::CreateTopLevelWindow(new BrowserWindowDelegate(browserView)) != nullptr;
}
