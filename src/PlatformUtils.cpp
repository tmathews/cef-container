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

void PlatformUtils::AdjustWindowSize(CefRefPtr<CefWindow> window, int contentWidth, int contentHeight)
{
#ifdef _WIN32
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = contentWidth;
	rc.bottom = contentHeight;

	auto hWnd = window->GetWindowHandle();
	auto style = GetWindowLong(hWnd, GWL_STYLE);

	AdjustWindowRect(&rc, style, FALSE);

	window->SetSize(CefSize(rc.right - rc.left, rc.bottom - rc.top));
#else
	window->SetSize(contentWidth, contentHeight);
#endif
}
