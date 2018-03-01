#include "StandardIoArchive.h"
#include "StandardIoFileStream.h"

#include <stdio.h>

StandardIoArchive::StandardIoArchive(const std::string& basePath)
	: m_basePath(basePath)
{
}

StandardIoArchive* StandardIoArchive::New(const std::string& basePath)
{
	return new StandardIoArchive(basePath);
}

IFileStream* StandardIoArchive::Open(const std::string& fileName)
{
	auto handle = fopen((m_basePath + fileName).c_str(), "rb");
	return handle ? new StandardIoFileStream(handle) : nullptr;
}

bool StandardIoArchive::Exists(const std::string& fileName)
{
	return false;
}

bool StandardIoArchive::List(const std::string& path, int filterMask, std::vector<std::string>& fileList)
{
	return true;
}

