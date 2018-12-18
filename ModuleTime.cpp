#include "ModuleTime.h"
#include "Application.h"

#include "mmgr\mmgr.h"



ModuleTime::ModuleTime()
{
	name = "Time manager";
	game_timer.Stop();
	game_time = 0;
	time_scale = 1;
	frame_count = 0;
}


ModuleTime::~ModuleTime()
{
}

void ModuleTime::Pause()
{
	GameState& current_state = App->current_game_state;
	switch (current_state)
	{
	case RUNNING:
		game_timer.Stop();
		break;

	case STOPPED:
		break;

	case PAUSED:
		game_timer.Resume();
		break;

	default:
		break;
	}
}

void ModuleTime::Play()
{
	GameState& current_state = App->current_game_state;
	switch (current_state)
	{
	case RUNNING:

		game_timer.Start();
		game_timer.Stop();
		game_time = 0;
		frame_count = 0;
		break;

	case STOPPED:
		game_timer.Start();
		game_time = 0;
		frame_count = 0;
		break;

	case PAUSED:
		current_state = STOPPED;
		game_timer.Start();
		game_timer.Stop();
		break;

	default:
		break;
	}
}

update_status ModuleTime::Update(float dt)
{
	GameState& current_state = App->current_game_state;
	if (current_state != PAUSED && current_state != STOPPED) {
		game_dt = (SDL_GetTicks() - last_frame_time)* time_scale;
		game_time += game_dt;
		frame_count++;
	}
	else
		game_dt = 0;

	last_frame_time = SDL_GetTicks();
	return update_status::UPDATE_CONTINUE;
}

void ModuleTime::RecieveEvent(const Event & event)
{
	switch (event.type)
	{
	case PLAY:
		Play();
		break;
	case PAUSE:
		Pause();
		break;
	default:
		break;
	}
}


