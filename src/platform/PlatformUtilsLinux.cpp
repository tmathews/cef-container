#include "PlatformUtils.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void PlatformUtils::MakeWindowResizable(CefRefPtr<CefWindow> window, bool resizable)
{
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
}

void PlatformUtils::SetWindowAllowedActions(CefRefPtr<CefWindow> window, bool maximizable, bool minimizable)
{
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
}

void PlatformUtils::AdjustWindowSize(CefRefPtr<CefWindow> window, int contentWidth, int contentHeight)
{
	window->SetSize(CefSize(contentWidth, contentHeight));
}
