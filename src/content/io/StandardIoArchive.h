#pragma once

#include "Archive.h"

class StandardIoArchive final : public IArchive
{
	IMPLEMENT_REFCOUNTING(StandardIoArchive);

private:
	explicit StandardIoArchive(const std::string& basePath);

public:
	static StandardIoArchive* New(const std::string& basePath);

	IFileStream* Open(const std::string& fileName) override;

	bool Exists(const std::string& fileName) override;

	bool List(const std::string& path, int filterMask, std::vector<std::string>& fileList) override;

private:
	std::string m_basePath;
};

