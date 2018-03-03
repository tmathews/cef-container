#include "AppConfig.h"

#include <stdio.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

bool LoadAppConfig(const char* fileName, AppConfig* config)
{
	auto file = fopen(fileName, "rb");
	if (!file)
	{
		fprintf(stderr, "could not open app config file at %s\n", fileName);
		return false;
	}

	rapidjson::Document document;
	
	char buffer[65536];
	rapidjson::FileReadStream stream(file, buffer, sizeof(buffer));

	document.ParseStream(stream);

	if (document.HasParseError())
	{
		fprintf(stderr, "config %s: got parse error: %s\n", fileName, rapidjson::GetParseError_En(document.GetParseError()));
		
		fclose(file);

		return false;
	}

	auto windowObject = document["window"].GetObject();

	strcpy(config->iconPath, windowObject["icon"].GetString());

	config->sizeWidth = windowObject["width"].GetInt();
	config->sizeHeight = windowObject["height"].GetInt();
	config->resizeEnabled = windowObject["resizable"].GetBool();

	auto fullscreenObject = document["fullscreen"].GetObject();
	config->fullscreenEnabled = fullscreenObject["enabled"].GetBool();
	config->fullscreenInitial = fullscreenObject["initial"].GetBool();
	config->fullscreenF11 = fullscreenObject["f11"].GetBool();
	config->fullscreenAltEnter = fullscreenObject["altEnter"].GetBool();

	config->contextmenuEnabled = document["contextMenu"]["enabled"].GetBool();

	config->exitEsc = document["exit"]["esc"].GetBool();

	auto archivesArray = document["content"]["archives"].GetArray();
	for (auto it = archivesArray.Begin(); it != archivesArray.End() && config->numArchives < MAX_ARCHIVES; ++it)
	{
		strncpy(config->archives[config->numArchives++], it->GetString(), sizeof(config->archives[0]));
	}

	fclose(file);

	return true;
}
