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
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fulldesktop);

	void SetLightSlider(float brightness);
	void SetWindowSize(int width, int height);

	void Save_Config(JSON_Object& config) const;
	void Load_Config(const JSON_Object& config);

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;

	float brightness;

	int window_width = SCREEN_WIDTH;
	int window_height = SCREEN_HEIGHT;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	const char* window_title;
};

#endif // __ModuleWindow_H__