#include "Content.h"
#include "FileUtils.h"

#include <include/cef_parser.h>

static CefRefPtr<ContentHandlerFactory> s_factoryInstance(new ContentHandlerFactory());

ContentResourceHandler::ContentResourceHandler()
	: m_file(nullptr)
	, m_totalRead(0)
{
}

ContentResourceHandler::~ContentResourceHandler()
{
	Cancel();
}

inline std::string GetFileExtension(const std::string &fileName)
{
	auto dotPos = fileName.find_last_of('.');
	return dotPos != std::string::npos ? fileName.substr(dotPos + 1) : "";
}

bool ContentResourceHandler::ProcessRequest(
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefCallback> callback
)
{
	auto url = request->GetURL();

	CefURLParts urlParts;
	if (!CefParseURL(request->GetURL(), urlParts))
		return false;

	CefString path;
	path.FromString(urlParts.path.str, urlParts.path.length, true);

	auto ext = GetFileExtension(path.ToString());

	m_mimeType = CefGetMimeType(ext);

	auto fileName = std::string("./content/") + path.ToString();

	m_file = fopen(fileName.c_str(), "rb");

	if (!m_file)
		return false;

	fseek(m_file, 0, SEEK_END);
	m_fileSize = ftell(m_file);
	fseek(m_file, 0, SEEK_SET);

	callback->Continue();

	return true;
}

void ContentResourceHandler::GetResponseHeaders(
	CefRefPtr<CefResponse> response,
	int64& response_length,
	CefString& redirectUrl
)
{
	response->SetMimeType(m_mimeType);
	response->SetStatus(200);
	response_length = m_fileSize;
}

bool ContentResourceHandler::ReadResponse(
	void* data_out,
	int bytes_to_read,
	int& bytes_read,
	CefRefPtr<CefCallback> callback
)
{
	bytes_read = (int)fread(data_out, 1, bytes_to_read, m_file);
	m_totalRead += bytes_read;
	return true;
}

void ContentResourceHandler::Cancel()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

CefRefPtr<ContentHandlerFactory> ContentHandlerFactory::Get()
{
	return s_factoryInstance;
}

CefRefPtr<CefResourceHandler> ContentHandlerFactory::Create(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString &scheme_name,
	CefRefPtr<CefRequest> request
)
{
	return new ContentResourceHandler();
}
