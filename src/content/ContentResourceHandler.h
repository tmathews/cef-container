#pragma once

#include <include/cef_scheme.h>

class IFileStream;

class ContentResourceHandler : public CefResourceHandler
{
	IMPLEMENT_REFCOUNTING(ContentResourceHandler);
	DISALLOW_COPY_AND_ASSIGN(ContentResourceHandler);

public:
	ContentResourceHandler(const std::string& mimeType, IFileStream* file, int size);
	
	~ContentResourceHandler();

	virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override;

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) override;

	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) override;

	virtual void Cancel() override;

private:
	CefString m_mimeType;
	IFileStream* m_file;
	int m_fileSize;
};
