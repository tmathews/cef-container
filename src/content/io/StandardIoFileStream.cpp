#include "StandardIoFileStream.h"

#include <stdio.h>

inline static int IoSeekOriginToStandard(IoSeekOrigin origin)
{
	switch (origin)
	{
	case IoSeekOrigin::Current:
		return SEEK_CUR;

	case IoSeekOrigin::Start:
		return SEEK_SET;

	case IoSeekOrigin::End:
		return SEEK_END;
	}
	return SEEK_SET;
}

StandardIoFileStream::StandardIoFileStream(FILE* handle)
	: m_handle(handle)
{
}

int StandardIoFileStream::Read(void* output, int byteCount)
{
	return (int)fread(output, 1, byteCount, m_handle);
}

int StandardIoFileStream::Tell()
{
	return ftell(m_handle);
}

void StandardIoFileStream::Seek(IoSeekOrigin origin, int offset)
{
	fseek(m_handle, offset, IoSeekOriginToStandard(origin));
}

bool StandardIoFileStream::IsEndOfFile()
{
	return feof(m_handle) != 0;
}

void StandardIoFileStream::Close()
{
	fclose(m_handle);

	delete this;
}
