#pragma once
#include "Module.h"
#include "Globals.h"


//PROVISIONAL
//#include "MathGeoLib\MathGeoLib.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	//float3 yesa;

	// Utility

	update_status DrawTopBar();
	void SetDefaultStyle(); 

};
