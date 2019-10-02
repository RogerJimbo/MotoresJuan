#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Config : public GUI_Element
{
public:
	GUI_Config(Application* app);
	~GUI_Config();

	void Draw();

private:
	bool active_engine_windows[NUM_ACT_WIN];
	bool close_tab = true;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldesktop = false;

	char* RDTSC;
	char* MMX;
	char* AVX;
	char* SSE;
	char* SSE2;
	char* SSE3;
	char* SSE41;
	char* SSE42;

	float fps[100];
	float ms[100];

	float bright_slider_scroll = 1;

	SDL_bool sdl_true = SDL_TRUE;
	SDL_bool sdl_false = SDL_FALSE;
};

