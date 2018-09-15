#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "imgui.h"


#define MAX_SNAKE 2


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

};
