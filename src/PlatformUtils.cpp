#include "PlatformUtils.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#endif

#ifndef _WIN32
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
#else
	auto hints = XAllocSizeHints();
	hints->flags = PMinSize | PMaxSize;
	if (resizable)
	{
		hints->min_width = 100;
		hints->min_height = 100;
		hints->max_width = 99999;
		hints->max_height = 99999;
	}
	else
	{
		auto windowSize = window->GetSize();
		hints->min_width = windowSize.width;
		hints->min_height = windowSize.height;
		hints->max_width = windowSize.width;
		hints->max_height = windowSize.height;
	}
	XSetWMNormalHints(cef_get_xdisplay(), window->GetWindowHandle(), hints);
	XFree(hints);
	printf("resizable: %s\n", resizable?"yes":"no");
#endif
}

void PlatformUtils::SetWindowAllowedActions(CefRefPtr<CefWindow> window, bool maximizable, bool minimizable)
{
#ifdef _WIN32
	UpdateWindowStyle(window, WS_MAXIMIZEBOX, maximizable);
	UpdateWindowStyle(window, WS_MINIMIZEBOX, minimizable);
#else
	auto display = cef_get_xdisplay();
	int numValues = 0;
	Atom values[4];
	if (maximizable)
	{
		values[numValues++] = XInternAtom(display, "_NET_WM_ACTION_MAXIMIZE_HORZ", True);
		values[numValues++] = XInternAtom(display, "_NET_WM_ACTION_MAXIMIZE_VERT", True);
	}
	if (minimizable)
	{
		values[numValues++] = XInternAtom(display, "_NET_WM_ACTION_MINIMIZE", True);
	}
	XChangeProperty(
		display,
		window->GetWindowHandle(),
		XInternAtom(display, "_NET_WM_ALLOWED_ACTIONS", True),
		XA_ATOM,
		32,
		PropModeReplace,
		(unsigned char*)values,
		numValues
	);
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
	window->SetSize(CefSize(contentWidth, contentHeight));
#endif
}
