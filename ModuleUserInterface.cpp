#include "ModuleUserInterface.h"
#include "UI_Image.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "GameObject.h"
#include "OpenGL.h"
#include "DebugDraw.h"

ModuleUserInterface::ModuleUserInterface()
{
	name = "User_Interface"; 
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
	ui_render_box.minPoint = { -1, -1, -1 };
	ui_render_box.maxPoint = { 1, 1, 1 };

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

	ComponentCamera* cam = (ComponentCamera*)camera->GetComponent(CMP_CAMERA); 

	if (cam->is_editor)
		editor_cam = true; 

	//Draw normal GameObjects
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{		
		if (!editor_cam)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			
			ComponentRectTransform* rtransform = (ComponentRectTransform*)(*it)->GetComponent(CMP_RECTTRANSFORM);
			Transform canvas_transform = rtransform->GetTransform()->transform; 

			// args: left, right, bottom, top, near, far
			float left = canvas_transform .position.x -rtransform->width / 2;
			float right = canvas_transform.position.x + rtransform->width / 2;
			float bottom = canvas_transform.position.y -rtransform->height / 2;
			float top = canvas_transform.position.y + rtransform->height / 2;
			float near_plane = 0.1f;
			float far_plane = -200.0f;

			glOrtho(left, right, bottom, top, near_plane, far_plane);

			float3 min = {left, bottom, near_plane}; 
			float3 max = { right, top, far_plane};
		 
			ui_render_box.minPoint = min; 
			ui_render_box.maxPoint = max;

			App->renderer3D->UseDebugRenderSettings(); 			
		}
		
		LineSegment curr_line;

		glBegin(GL_LINES);
		App->renderer3D->UseDebugRenderSettings();
		glColor3f(1.0f, 0.0f, 0.0f);

		for (int i = 0; i < 12; i++)
		{
			curr_line = ui_render_box.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}

		glEnd();
	
		App->renderer3D->UseUIRenderSettings();
		(*it)->Draw(editor_cam);
	}
}

void ModuleUserInterface::AddCanvas(GameObject* canvas_go)
{
	go_with_canvas.push_back(canvas_go); 
}

void ModuleUserInterface::DeleteCanvas(GameObject * go)
{
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{
		if ((*it) == go)
		{
			go_with_canvas.erase(it); 
			return; 
		}
	}
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
