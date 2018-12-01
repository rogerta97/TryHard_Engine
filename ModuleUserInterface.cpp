#include "ModuleUserInterface.h"
#include "UI_Image.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"
#include "ComponentCamera.h"
#include "GameObject.h"

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

void ModuleUserInterface::DrawSceneUI(GameObject* camera)
{
	bool editor_cam = false;

	//Draw normal GameObjects
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{		
		(*it)->Draw(editor_cam);
	}
}

void ModuleUserInterface::AddCanvas(GameObject* canvas_go)
{
	go_with_canvas.push_back(canvas_go); 
}

void ModuleUserInterface::AddaptCanvasToScreen()
{
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{
		ComponentRectTransform* r_transform = (ComponentRectTransform*)(*it)->GetComponent(CMP_RECTTRANSFORM);
		if (r_transform != nullptr)
		{
			r_transform->AddaptRectToScreenSize();
		}
	}
}

GameObject* ModuleUserInterface::GetLastCanvas() const
{
	int limit = go_with_canvas.size(); 
	int count = 0; 

	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{
		if (++count == limit)
			return (*it); 
	}

	return nullptr; 
}
