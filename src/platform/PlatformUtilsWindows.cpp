#include "PlatformUtils.h"

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

static void UpdateWindowStyle(CefRefPtr<CefWindow> window, DWORD dwStyle, bool enable)
{
	auto hWnd = window->GetWindowHandle();
	auto dwNewStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (enable)
	{
		dwNewStyle |= dwStyle;
	}
	else
	{
		dwNewStyle &= ~dwStyle;
	}
	::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
}

void PlatformUtils::MakeWindowResizable(CefRefPtr<CefWindow> window, bool resizable)
{
	UpdateWindowStyle(window, WS_SIZEBOX, resizable);
}

void PlatformUtils::SetWindowAllowedActions(CefRefPtr<CefWindow> window, bool maximizable, bool minimizable)
{
	UpdateWindowStyle(window, WS_MAXIMIZEBOX, maximizable);
	UpdateWindowStyle(window, WS_MINIMIZEBOX, minimizable);
}

void PlatformUtils::AdjustWindowSize(CefRefPtr<CefWindow> window, int contentWidth, int contentHeight)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = contentWidth;
	rc.bottom = contentHeight;

	auto hWnd = window->GetWindowHandle();
	auto style = GetWindowLong(hWnd, GWL_STYLE);

	AdjustWindowRect(&rc, style, FALSE);

	window->SetSize(CefSize(rc.right - rc.left, rc.bottom - rc.top));
}
