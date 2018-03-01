#pragma once

#include <include/cef_scheme.h>

class ContentHandlerFactory : public CefSchemeHandlerFactory
{
	IMPLEMENT_REFCOUNTING(ContentHandlerFactory);

public:
	virtual CefRefPtr<CefResourceHandler> Create(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& scheme_name,
		CefRefPtr<CefRequest> request
	) override;
};
