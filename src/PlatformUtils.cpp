#include "PlatformUtils.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#endif

#ifdef _WIN32
static void UpdateWindowStyle(CefRefPtr<CefWindow> window, DWORD dwStyle, bool enable)
{
	auto hWnd = window->GetWindowHandle();
	auto dwNewStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (enable)
		dwNewStyle |= dwStyle;
	else
		dwNewStyle &= ~dwStyle;
	::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
}
#endif

void PlatformUtils::MakeWindowResizable(CefRefPtr<CefWindow> window, bool resizable)
{
#ifdef _WIN32
	UpdateWindowStyle(window, WS_SIZEBOX, resizable);
#endif
}

void PlatformUtils::MakeWindowMaximizable(CefRefPtr<CefWindow> window, bool maximizable)
{
#ifdef _WIN32
	UpdateWindowStyle(window, WS_MAXIMIZEBOX, maximizable);
#endif
}

void PlatformUtils::MakeWindowMinimizable(CefRefPtr<CefWindow> window, bool minimizable)
{
#ifdef _WIN32
	UpdateWindowStyle(window, WS_MINIMIZEBOX, minimizable);
#endif
}
