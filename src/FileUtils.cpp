#include "FileUtils.h"

#include <stdio.h>

CefString FileUtils::ReadWholeTextFile(const CefString &fileName)
{
	FILE *fp = fopen(fileName.ToString().c_str(), "rb");
	if (fp == nullptr)
		return "";

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, 1, size, fp);
	data[size] = 0;

	fclose(fp);

	CefString str(data);

	delete[] data;

	return str;
}
