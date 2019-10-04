#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow(Application* app, bool start_enabled = true);

	virtual ~ModuleWindow();

	bool Init(const JSON_Object& config);
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);

	void Save_Config(JSON_Object& config) const;

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;

	int window_width = SCREEN_WIDTH;
	int window_height = SCREEN_HEIGHT;

	bool fullscreenconfig;
};

#endif // __ModuleWindow_H__