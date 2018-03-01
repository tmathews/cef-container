#pragma once

#include "FileStream.h"

#include <stdio.h>

class StandardIoFileStream final : public IFileStream
{
public:
	explicit StandardIoFileStream(FILE* handle);

	int Read(void* output, int byteCount) override;

	int Tell() override;

	void Seek(IoSeekOrigin origin, int offset) override;

	bool IsEndOfFile() override;

	void Close() override;

private:
	FILE* m_handle;
};

