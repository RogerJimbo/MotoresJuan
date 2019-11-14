#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer 
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Reset();
	void Pause();
	void Resume();


	Uint32 ReadTime() const;

private:

	bool	running = false;
	bool	reset = false;

	Uint32	started_at;
	Uint32	stopped_at;
	Uint32 paused_time;
};

#endif //__TIMER_H__