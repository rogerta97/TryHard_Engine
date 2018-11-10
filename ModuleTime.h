#pragma once
#include "Module.h"
class ModuleTime :
	public Module
{
public:
	ModuleTime();
	~ModuleTime();

	Timer real_timer;
	Timer game_timer;

	void Pause();
	void Play();

	update_status Update(float dt) override;

	//Events
	void RecieveEvent(const Event& event) override;
};

