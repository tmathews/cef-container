#pragma once

#include <include/cef_scheme.h>
#include <stdio.h>

class ContentResourceHandler : public CefResourceHandler
{
	IMPLEMENT_REFCOUNTING(ContentResourceHandler);
	DISALLOW_COPY_AND_ASSIGN(ContentResourceHandler);

public:
	ContentResourceHandler();
	~ContentResourceHandler();

	virtual bool ProcessRequest(
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback
	) override;

	virtual void GetResponseHeaders(
		CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl
	) override;

	virtual bool ReadResponse(
		void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback
	) override;

	virtual void Cancel() override;

private:
	FILE *m_file;
	int64 m_fileSize;
	int64 m_totalRead;
	CefString m_mimeType;
};

class ContentHandlerFactory : public CefSchemeHandlerFactory
{
	IMPLEMENT_REFCOUNTING(ContentHandlerFactory);

public:
	static CefRefPtr<ContentHandlerFactory> Get();

	virtual CefRefPtr<CefResourceHandler> Create(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& scheme_name,
		CefRefPtr<CefRequest> request
	) override;
};
