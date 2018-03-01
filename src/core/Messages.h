#pragma once

#include <include/internal/cef_string.h>


// Sent from browser process to render process to have it visit the
// index page document.
extern const CefString kMessageVisitIndex;

// Sent from the renderer process to apply a config.
extern const CefString kMessageConfigure;

enum
{
	// Dictionary passed as an argument to kMsgConfigPayload containing
	// the various parameters.
	kParamConfig
};
