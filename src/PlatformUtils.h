#pragma once

#include <include/views/cef_window.h>

class PlatformUtils
{
public:
	static void MakeWindowResizable(CefRefPtr<CefWindow> window, bool resizable = true);

	static void MakeWindowMaximizable(CefRefPtr<CefWindow> window, bool maximizable = true);

	static void MakeWindowMinimizable(CefRefPtr<CefWindow> window, bool minimizable = true);
};
