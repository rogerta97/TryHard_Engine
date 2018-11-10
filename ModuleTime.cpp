#include "ModuleTime.h"
#include "Application.h"



ModuleTime::ModuleTime()
{
	name = "Time manager";
	game_timer.Stop();
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
		current_state = PAUSED;
		game_timer.Stop();
		break;

	case STOPPED:
		break;

	case PAUSED:
		current_state = RUNNING;
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
		break;

	case STOPPED:
		game_timer.Start();

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
	return update_status::UPDATE_CONTINUE;
}

void ModuleTime::RecieveEvent(const Event & event)
{

	switch (event.type)
	{
	case Event::PLAY:
		Play();
		break;
	case Event::PAUSE:
		Pause();
		break;
	default:
		break;
	}
}


