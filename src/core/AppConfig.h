#pragma once

#include <string>
#include <vector>

#define MAX_ARCHIVES 64

struct AppConfig
{
	char iconPath[128] = { 0 };
	bool fullscreenEnabled = true;
	bool fullscreenInitial = false;
	char fullscreenKey[32] = { 0 };
	bool fullscreenAltEnter = false;
	bool fullscreenF11 = true;
	bool exitEsc = false;
	int sizeWidth = 800;
	int sizeHeight = 600;
	bool maximizeEnabled = true;
	bool minimizeEnabled = true;
	bool resizeEnabled = true;
	bool contextmenuEnabled = false;
	char archives[MAX_ARCHIVES][128];
	int numArchives = 0;
};

bool LoadAppConfig(const char* fileName, AppConfig* config);
