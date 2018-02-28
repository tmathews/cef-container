#include "Content.h"

#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>

ContentResourceHandler::ContentResourceHandler(const std::string &mimeType, FILE *file, int size)
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

inline std::string GetFileExtension(const std::string &fileName)
{
	auto dotPos = fileName.find_last_of('.');
	return dotPos != std::string::npos ? fileName.substr(dotPos + 1) : "";
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

CefRefPtr<CefResourceHandler> ContentHandlerFactory::Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString &scheme_name, CefRefPtr<CefRequest> request)
{
	CEF_REQUIRE_IO_THREAD();

	auto url = request->GetURL();

	CefURLParts urlParts;
	if (!CefParseURL(request->GetURL(), urlParts))
	{
		return false;
	}

	CefString pathTmp;
	pathTmp.FromString(urlParts.path.str, urlParts.path.length, true);
	pathTmp = CefURIDecode(pathTmp, true, (cef_uri_unescape_rule_t)(UU_NORMAL | UU_SPACES));

	if (pathTmp.empty())
	{
		return false;
	}

	std::string path = pathTmp.ToString();

	pathTmp.ClearAndFree();

	if (path.back() == '/' || path.back() == '\\')
	{
		path = path + "index.html";
	}

	auto ext = GetFileExtension(path);

	auto mimeType = CefGetMimeType(ext);

	if (mimeType.empty())
	{
		if (ext.compare("json") == 0)
		{
			mimeType = "text/javascript";
		}
		else if (ext.compare("ts") == 0)
		{
			mimeType = "text/x.typescript";
		}
	}

	auto fileName = std::string("./content") + path;

	auto file = fopen(fileName.c_str(), "rb");
	if (!file)
	{
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	auto fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (fileSize == 0)
	{
		fclose(file);
		return nullptr;
	}

	return new ContentResourceHandler(mimeType, file, fileSize);
}
