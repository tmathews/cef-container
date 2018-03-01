#include "ContentHandlerFactory.h"
#include "ContentResourceHandler.h"

#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>

inline std::string GetFileExtension(const std::string& fileName)
{
	auto dotPos = fileName.find_last_of('.');
	return dotPos != std::string::npos ? fileName.substr(dotPos + 1) : "";
}

CefRefPtr<CefResourceHandler> ContentHandlerFactory::Create(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& scheme_name,
	CefRefPtr<CefRequest> request
)
{
	CEF_REQUIRE_IO_THREAD();

	auto url = request->GetURL();

	CefURLParts urlParts;
	if (!CefParseURL(request->GetURL(), urlParts))
	{
		return nullptr;
	}

	CefString pathTmp;
	pathTmp.FromString(urlParts.path.str, urlParts.path.length, true);
	pathTmp = CefURIDecode(pathTmp, true, (cef_uri_unescape_rule_t)(UU_NORMAL | UU_SPACES));

	if (pathTmp.empty())
	{
		return nullptr;
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
