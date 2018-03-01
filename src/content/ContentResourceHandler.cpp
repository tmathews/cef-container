#include "ContentResourceHandler.h"

#include "io/FileStream.h"

#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>

ContentResourceHandler::ContentResourceHandler(const std::string& mimeType, IFileStream* file, int size)
	: m_mimeType(mimeType)
	, m_file(file)
	, m_fileSize(size)
{
}

ContentResourceHandler::~ContentResourceHandler()
{
	if (m_file)
	{
		m_file->Close();
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

	if (m_file->IsEndOfFile())
	{
		return false;
	}

	bytes_read = m_file->Read(data_out, bytes_to_read);

	return true;
}

void ContentResourceHandler::Cancel()
{
	CEF_REQUIRE_IO_THREAD();

	if (m_file)
	{
		m_file->Close();
		m_file = nullptr;
	}
}

