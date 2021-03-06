#pragma once

#include "io/Archive.h"

#include <include/cef_scheme.h>

#include <vector>

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

	void MountArchive(CefRefPtr<IArchive> archive);

private:
	std::vector<CefRefPtr<IArchive>> m_archives;
};
