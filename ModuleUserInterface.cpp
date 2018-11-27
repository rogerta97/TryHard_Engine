#include "ModuleUserInterface.h"
#include "UI_Image.h"

ModuleUserInterface::ModuleUserInterface()
{
}


ModuleUserInterface::~ModuleUserInterface()
{
}

bool ModuleUserInterface::Init(JSON_Object * config)
{
	return true;
}

bool ModuleUserInterface::Start()
{
	return true;
}

update_status ModuleUserInterface::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleUserInterface::CleanUp()
{
	return true;
}

UI_Element * ModuleUserInterface::CreateUIElement(UI_Widgget_Type type)
{
	UI_Element* to_ret = nullptr; 

	switch (type)
	{
	case UI_Widgget_Type::UI_BUTTON:
		// TODO :)
		break; 

	case UI_Widgget_Type::UI_CHECKBOX:
		// TODO :)
		break;

	case UI_Widgget_Type::UI_INPUTFIELD:
		// TODO :)
		break;

	case UI_Widgget_Type::UI_LABEL:
		// TODO :)
		break;

	case UI_Widgget_Type::UI_IMAGE:
		to_ret = new UI_Image(); 
		break;

	}

	return to_ret;
}
