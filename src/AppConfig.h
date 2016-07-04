#pragma once

#include <string>

struct AppConfig
{
	bool fullscreenEnabled = true;
	bool fullscreenInitial = false;
	char fullscreenKey[32] = { 0 };
	bool fullscreenAltEnter = false;
	bool fullscreenF11 = false;
	int sizeWidth = 800;
	int sizeHeight = 600;
	bool maximizeEnabled = true;
	bool minimizeEnabled = true;
	bool resizeEnabled = true;
	bool contextmenuEnabled = false;
};
