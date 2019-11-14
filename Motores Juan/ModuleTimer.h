#pragma once
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

enum GameState { STOP, PAUSE, PLAY };

class OwnTimer
{
public:
	OwnTimer() {}
	~OwnTimer() {}

	void Start() { game_paused = false; time = 0; lastframetime = SDL_GetTicks(); }
	void Update();
	void Pause() { game_paused = true; }
	void Resume() { game_paused = false; }
	void Reset() { time = 0; game_paused = true; }

	float ReadTime() { return time; }
	float ReadDeltaTime() { return deltatime; }

	float getScale() { return timescale; }
	void setScale(float scale) { timescale = scale; }


private:

	float time = 0;
	float timescale = 1;
	float deltatime = 0;
	float lastframetime = 0;

	bool game_paused = true;
};

class ModuleTimer : public Module
{
public:

	ModuleTimer(Application* app, bool start_enabled = true);
	~ModuleTimer();


	bool Init(const JSON_Object* config);
	
	update_status Update(float dt);

	bool CleanUp() { return true; }

	void Play() { gamestate = PLAY; };
	void Pause() { gamestate = PAUSE; };
	void Stop() { gamestate = STOP; };

	GameState GetGameState() { return gamestate; }

	float getRealTime() { return RealTime.ReadTime(); }
	float getGameTime() { return GameTime.ReadTime(); }
	float getGameDeltaTime() { return DeltaTimer.ReadTime(); }

	float GetDeltaTime();
	float GetGameDeltaTime() { return GameTime.ReadDeltaTime(); }


	int getFrameCount() { return framecounter; }
	void setTimeScale(float scale) { GameTime.setScale(scale); }

private:

	void HandlePlayMode();
	void HandlePauseMode();
	void HandleStopMode();

	GameState gamestate = GameState::STOP;
	GameState laststate = GameState::STOP;

	Timer RealTime;
	Timer DeltaTimer;

	OwnTimer GameTime;

	uint framecounter = 0;

	float dt;

};