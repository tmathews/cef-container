#include "ContentResourceHandler.h"

#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>

ContentResourceHandler::ContentResourceHandler(const std::string& mimeType, FILE* file, int size)
	: m_mimeType(mimeType)
	, m_file(file)
	, m_fileSize(size)
{
}

ContentResourceHandler::~ContentResourceHandler()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

bool ContentResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback)
{
	CEF_REQUIRE_IO_THREAD();

	callback->Continue();

	return true;
}

void ContentResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl)
{
	CEF_REQUIRE_IO_THREAD();

	if (!m_mimeType.empty())
	{
		response->SetMimeType(m_mimeType);
	}

	response->SetStatus(200);
	response->SetStatusText("OK");

	response_length = m_fileSize;
}

bool ContentResourceHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback)
{
	CEF_REQUIRE_IO_THREAD();

	if (feof(m_file))
	{
		return false;
	}

	bytes_read = (int)fread(data_out, 1, bytes_to_read, m_file);

	return true;
}

void ContentResourceHandler::Cancel()
{
	CEF_REQUIRE_IO_THREAD();

	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

