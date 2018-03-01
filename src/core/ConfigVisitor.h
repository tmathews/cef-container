#pragma once

#include "include/cef_dom.h"
#include "include/cef_browser.h"

class ConfigVisitor : public CefDOMVisitor
{
	IMPLEMENT_REFCOUNTING(ConfigVisitor);

public:
	ConfigVisitor(CefRefPtr<CefBrowser> browser);

	virtual void Visit(CefRefPtr<CefDOMDocument> document) override;

private:
	CefRefPtr<CefBrowser> m_browser;
};
