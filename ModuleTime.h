#ifndef _MODULETIME_H_
#define _MODULETIME_H_

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

	Uint32 game_time;
	Uint32 last_frame_time;
	float game_dt;

	uint frame_count;

	float time_scale;

	//Events
	void RecieveEvent(const Event& event) override;
};

#endif

