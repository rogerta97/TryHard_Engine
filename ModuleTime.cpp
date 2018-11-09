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

update_status ModuleTime::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

void ModuleTime::RecieveEvent(const Event & event)
{
	switch (event.type)
	{
	case Event::PLAY:
		game_timer.Start();
		break;
	case Event::PAUSE:
		game_timer.Stop();
		break;
	default:
		break;
	}
}


