#pragma once

#include <include/internal/cef_string.h>


// Sent from browser process to render process to have it visit the
// index page document.
extern const CefString kMsgIndexLoaded;

// Sent from the renderer process to apply a config.
extern const CefString kMsgConfigPayload;

enum
{
	// Dictionary passed as an argument to kMsgConfigPayload containing
	// the various parameters.
	kParamConfig
};
