#pragma once

#include "Module.h"
#include "UI_Element.h"

class ModuleUserInterface : public Module
{
public:
	ModuleUserInterface();
	~ModuleUserInterface();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	UI_Element* CreateUIElement(UI_Widgget_Type type = UI_Widgget_Type::UI_NULL);
};

