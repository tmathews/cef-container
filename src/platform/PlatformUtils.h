#pragma once

#include <include/views/cef_window.h>

class PlatformUtils
{
public:
	static void MakeWindowResizable(CefRefPtr<CefWindow> window, bool resizable);

	static void SetWindowAllowedActions(CefRefPtr<CefWindow> window, bool maximizable, bool minimizable);

	static void MakeWindowMinimizable(CefRefPtr<CefWindow> window, bool minimizable);

	static void AdjustWindowSize(CefRefPtr<CefWindow> window, int contentWidth, int contentHeight);
};
