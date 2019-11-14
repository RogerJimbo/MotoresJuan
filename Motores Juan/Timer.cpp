#include "Timer.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

void Timer::Reset()
{
	reset = true;
}

void Timer::Pause() 
{
	stopped_at = SDL_GetTicks();
	running = false;
}

void Timer::Resume() 
{
	paused_time += (SDL_GetTicks() - stopped_at);
	running = true;
}


Uint32 Timer::ReadTime() const
{
	if (!reset)
	{
		if (running == true) { return SDL_GetTicks() - started_at; }
		else { return stopped_at - started_at; }
	}
	else return 0;
}


