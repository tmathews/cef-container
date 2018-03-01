#include "ContentHandlerFactory.h"
#include "ContentResourceHandler.h"

#include "io/Archive.h"
#include "io/FileStream.h"

#include <include/cef_parser.h>
#include <include/wrapper/cef_helpers.h>

#include <algorithm>

inline std::string GetFileExtension(const std::string& fileName)
{
	auto dot = fileName.find_last_of('.');
	return dot != std::string::npos ? fileName.substr(dot + 1) : "";
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

	CefString temporaryPath;
	temporaryPath.FromString(urlParts.path.str, urlParts.path.length, true);
	temporaryPath = CefURIDecode(temporaryPath, true, (cef_uri_unescape_rule_t)(UU_NORMAL | UU_SPACES));

	if (temporaryPath.empty())
	{
		return nullptr;
	}

	std::string path = temporaryPath.ToString();

	temporaryPath.ClearAndFree();

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

	for (auto it = m_archives.rbegin(); it != m_archives.rend(); ++it)
	{
		auto archive = *it;

		auto file = archive->Open(path);
		if (!file)
		{
			continue;
		}
		
		file->Seek(IoSeekOrigin::End);
		auto fileSize = file->Tell();
		file->Seek(IoSeekOrigin::Start);
		
		return new ContentResourceHandler(mimeType, file, fileSize);
	}

	return nullptr;
}

void ContentHandlerFactory::MountArchive(CefRefPtr<IArchive> archive)
{
	if (std::find(m_archives.begin(), m_archives.end(), archive) == m_archives.end())
	{
		m_archives.push_back(archive);
	}
}
