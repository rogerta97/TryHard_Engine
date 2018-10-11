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

enum UpdateType { TYPE_PREUPDATE, TYPE_UPDATE, TYPE_POSTUPDATE };

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

	virtual void SaveConfigData(JSON_Object* config) {}

	void ManageMsBuffer(UpdateType type) {

		switch (type)
		{
		case TYPE_PREUPDATE:
			if (preupdate_ms_buffer.size() > 50)
				preupdate_ms_buffer.erase(preupdate_ms_buffer.begin());
			preupdate_ms_buffer.push_back(performance_timer.Read());
			break;
		case TYPE_UPDATE:
			if (update_ms_buffer.size() > 50)
				update_ms_buffer.erase(update_ms_buffer.begin());
			update_ms_buffer.push_back(performance_timer.Read());
			break;
		case TYPE_POSTUPDATE:
			if (postupdate_ms_buffer.size() > 50)
				postupdate_ms_buffer.erase(postupdate_ms_buffer.begin());
			postupdate_ms_buffer.push_back(performance_timer.Read());
			break;
		default:
			break;
		}

	}

	public:
	Timer performance_timer;

	int init_time;
	int start_time;

	std::vector<float> update_ms_buffer;
	std::vector<float> preupdate_ms_buffer;
	std::vector<float> postupdate_ms_buffer;
};

extern Application* App;