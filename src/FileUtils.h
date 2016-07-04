#pragma once

#include <include/internal/cef_string.h>

class FileUtils
{
public:
	static CefString ReadWholeTextFile(const CefString &fileName);
};
