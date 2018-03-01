#pragma once

#include <include/cef_base.h>

#include <string>
#include <vector>

enum
{
	kArchiveFilter_Directories = (1 << 0),
	kArchiveFilter_Files = (1 << 1)
};

class IFileStream;

class IArchive : public CefBaseRefCounted
{
public:
	virtual ~IArchive() {}

	virtual IFileStream* Open(const std::string& fileName) = 0;

	virtual bool Exists(const std::string& fileName) = 0;

	virtual bool List(const std::string& path, int filterMask, std::vector<std::string>& fileList) = 0;
};
