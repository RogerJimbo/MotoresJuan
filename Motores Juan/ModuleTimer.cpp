#include "ModuleTimer.h"
#include "Application.h"

ModuleTimer::ModuleTimer(Application* app, bool start_enabled) : Module(app, start_enabled) { module_name = "Time Manager"; }
ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::Init(const JSON_Object* config)
{
	GameTime.Reset();
	GameTime.setScale(1.f);
	RealTime.Reset();

	return true;
}

void ModuleTimer::HandlePlayMode()
{
	bool changedstate = false;

	switch (laststate)
	{
	case PAUSE:
		GameTime.Resume();
		changedstate = true;
		break;
	case STOP:
		RealTime.Start();
		GameTime.Start();
		changedstate = true;
		break;
	}

	++framecounter;
	if (changedstate) laststate = gamestate;
}

void ModuleTimer::HandlePauseMode()
{
	bool changedstate = false;

	switch (laststate)
	{
	case PLAY:
		GameTime.Pause();
		changedstate = true;
		break;
	case STOP:
		gamestate = STOP;
		return;
		break;
	}

	++framecounter;
	if (changedstate) laststate = gamestate;
}

void ModuleTimer::HandleStopMode()
{
	bool changedstate = false;

	switch (laststate)
	{
	case PAUSE || PLAY:
		RealTime.Reset();
		GameTime.Reset();
		framecounter = 0;
		gamestate = STOP;
		changedstate = true;
		break;
	}

	if (changedstate) laststate = gamestate;
}

update_status ModuleTimer::Update(float dt)
{
	switch (gamestate) 
	{
	case GameState::PLAY:
		HandlePlayMode();
		break;
	case GameState::PAUSE:
		HandlePauseMode();
		break;
	case GameState::STOP:
		HandleStopMode();
		break;
	}


	GameTime.Update(); 

	dt = DeltaTimer.ReadTime();
	DeltaTimer.Start();

	return UPDATE_CONTINUE;
}

void OwnTimer::Update()
{
	if (game_paused) deltatime = 0;
	else
	{
		deltatime = (SDL_GetTicks() - lastframetime) * deltatime;
		time += deltatime;
	}

	lastframetime = SDL_GetTicks();
}

float ModuleTimer::GetDeltaTime()
{
	float ret;
	return gamestate != STOP ? ret = dt : ret = 0;
}