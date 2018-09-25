#pragma once

#include "Globals.h"
#include "PCGRandom\pcg_basic.h"
#include "Timer.h"

#include <list>
#include <vector>

#include "JSON\parson.h"

class Application;
//struct JSON_Object;
struct PhysBody3D;

class Module
{
private : 
	bool enabled;

public:
	const char* name = "";

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* config)
	{
		init_time = performance_timer.Read();
		return true; 
	}

	virtual bool Start()
	{
		start_time = performance_timer.Read();
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void PrintConfigData() 
	{}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	void ManageMsBuffer() {
		if (runtime_ms_buffer.size() > 50)
			runtime_ms_buffer.erase(runtime_ms_buffer.begin());
		runtime_ms_buffer.push_back(performance_timer.Read()); 
	}

	public:
	Timer performance_timer;

	int init_time;
	int start_time;
	std::vector<float> runtime_ms_buffer;
};

extern Application* App;