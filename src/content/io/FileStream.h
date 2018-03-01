#pragma once

enum class IoSeekOrigin
{
	Current,
	Start,
	End,
};

class IFileStream
{
public:
	virtual ~IFileStream() {}

	virtual int Read(void* output, int byteCount) = 0;

	virtual void Seek(IoSeekOrigin origin, int offset = 0) = 0;

	virtual int Tell() = 0;

	virtual bool IsEndOfFile() = 0;

	virtual void Close() = 0;
};
