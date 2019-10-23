	#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	memset(player_controll, KEY_IDLE, sizeof(KEY_STATE) * MAX_BUTTONS);

	for (uint i = 0; i < MAX_BUTTONS; ++i) { player_controll[i] = KEY_IDLE; }
}

ModuleInput::~ModuleInput() { delete[] keyboard; }

bool ModuleInput::Init(const JSON_Object& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);
	SDL_Init(SDL_INIT_GAMECONTROLLER);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) { LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError()); ret = false; }
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) { fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError()); 	exit(1); }

	if (SDL_NumJoysticks() < 1) { LOG("Warning: No joysticks connected!\n"); }
	else { controller = SDL_GameControllerOpen(0); }

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();
	Uint8 pad_buttons[MAX_BUTTONS];
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	//GAME PAD

	pad_buttons[0] = SDL_GameControllerGetButton(this->controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
	pad_buttons[1] = SDL_GameControllerGetButton(this->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	pad_buttons[2] = SDL_GameControllerGetButton(this->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	pad_buttons[3] = SDL_GameControllerGetButton(this->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);

	for (int i = 0; i < MAX_BUTTONS; ++i)
	{
		if (pad_buttons[i] == 1)
		{
			if (player_controll[i] == KEY_IDLE) { player_controll[i] = KEY_DOWN; }
			else { player_controll[i] = KEY_REPEAT; }
		}
		else
		{
			if (player_controll[i] == KEY_REPEAT || player_controll[i] == KEY_DOWN) { player_controll[i] = KEY_UP; }
			else { player_controll[i] = KEY_IDLE; }
		}
	}

	//KEYBOARD
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)	keyboard[i] = KEY_DOWN;
			else keyboard[i] = KEY_REPEAT;	
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) keyboard[i] = KEY_UP;
			else keyboard[i] = KEY_IDLE;
		}
	}

	//MOUSE
	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE) mouse_buttons[i] = KEY_DOWN;	
			else mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN) { mouse_buttons[i] = KEY_UP; }
			else { mouse_buttons[i] = KEY_IDLE; }		
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_CONTROLLERBUTTONDOWN:
			move_up = true;
			break;

		case SDL_CONTROLLERBUTTONUP:
			move_up = false;
			break;

		case SDL_MOUSEWHEEL:
			mouse_z = event.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = event.motion.x / SCREEN_SIZE;
			mouse_y = event.motion.y / SCREEN_SIZE;

			mouse_x_motion = event.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = event.motion.yrel / SCREEN_SIZE;
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)	App->renderer3D->OnResize(event.window.data1, event.window.data2);
			break;

		case SDL_DROPFILE:
			string dropped_file = event.drop.file;
			size_t i = dropped_file.rfind('.', dropped_file.length());

			if (i != string::npos) { dropped_file = dropped_file.substr(i + 1, dropped_file.length() - i); }
			if (dropped_file == "fbx" || dropped_file == "FBX") { App->loader->Import(event.drop.file); }
			else if (dropped_file == "png" || dropped_file == "dds") { App->loader->Texturing(event.drop.file); }
			else { LOG("Unsupported file format"); }

			break;
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_F4] && keyboard[SDL_SCANCODE_LALT] == KEY_UP) { return UPDATE_STOP; }
		
	return UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_GameControllerClose(controller);
	controller = NULL;
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}