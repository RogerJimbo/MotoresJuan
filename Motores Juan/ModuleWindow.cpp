#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window, screen_surface = NULL;
	module_name = "Window"; 
}

ModuleWindow::~ModuleWindow() {}

bool ModuleWindow::Init(const JSON_Object& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * 1.4f;
		int height = SCREEN_HEIGHT * 1.2f;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Attributes OpenGL 
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true) { flags |= SDL_WINDOW_FULLSCREEN; }
		if(WIN_RESIZABLE == true) { flags |= SDL_WINDOW_RESIZABLE; }
		if (WIN_BORDERLESS == true) { flags |= SDL_WINDOW_BORDERLESS; }
		if(WIN_FULLSCREEN_DESKTOP == true) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else { screen_surface = SDL_GetWindowSurface(window); }
	}
	return ret;
}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	if(window != NULL) { SDL_DestroyWindow(window); }
	SDL_Quit();			//Quit SDL subsystems
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title); 
	window_title = title;
}

void ModuleWindow::SetLightSlider(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
	this->brightness = brightness;
}

void ModuleWindow::SetWindowSize(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
	this->window_height = height, this->window_width = width;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
	SDL_SetWindowFullscreen(window,flags);
	this->fullscreen = fullscreen;
	SDL_GetWindowSize(window, &window_width, &window_height);
}

void ModuleWindow::SetResizable(bool resizable)
{
	resizable ? SDL_WINDOW_RESIZABLE, SDL_SetWindowResizable(window, (SDL_TRUE)) : 0, SDL_SetWindowResizable(window, (SDL_FALSE));
	this->resizable = resizable;
	SDL_GetWindowSize(window, &window_width, &window_height);
}

void ModuleWindow::SetBorderless(bool borderless)
{
	borderless ? SDL_SetWindowBordered(window, SDL_FALSE) :  SDL_SetWindowBordered(window, (SDL_TRUE));
	this->borderless = borderless;
	SDL_GetWindowSize(window, &window_width, &window_height);
}

void ModuleWindow::SetFullDesktop(bool fulldesktop)
{
	Uint32 flags = fulldesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
	SDL_SetWindowFullscreen(window, flags);
	this->fulldesktop = fulldesktop;
	SDL_GetWindowSize(window, &window_width, &window_height);
}

void ModuleWindow::Save_Config(JSON_Object& config) const
{
	json_object_set_boolean(&config, "fullscreen", fullscreen);
	json_object_set_boolean(&config, "resizable", resizable);
	json_object_set_boolean(&config, "borderless", borderless);
	json_object_set_boolean(&config, "fulldesktop", fulldesktop);

	json_object_set_number(&config, "brightness", brightness);
	json_object_set_number(&config, "height", window_height);
	json_object_set_number(&config, "width", window_width);
}

 void ModuleWindow::Load_Config(const JSON_Object& config)
 {
	 //SetFullscreen(json_object_get_boolean(&config, "fullscreen"));
	 SetResizable(json_object_get_boolean(&config, "resizable"));
	 SetBorderless(json_object_get_boolean(&config, "borderless"));
	 SetFullDesktop(json_object_get_boolean(&config, "fulldesktop"));	

	// SetLightSlider(json_object_get_number(&config, "brightness"));
	// SetWindowSize(json_object_get_number(&config, "width"), json_object_get_number(&config, "height"));
 }