#include "ModuleUserInterface.h"
#include "UI_Image.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "GameObject.h"
#include "OpenGL.h"
#include "DebugDraw.h"
#include "Font.h"


ModuleUserInterface::ModuleUserInterface()
{
	name = "User_Interface"; 
}

ModuleUserInterface::~ModuleUserInterface()
{
}

bool ModuleUserInterface::Init(JSON_Object * config)
{
	FT_Error error = FT_Init_FreeType(&ft_library);

	if (error)
	{
		CONSOLE_ERROR("... an error occurred during FONT library initialization ..."); 
	}

	LoadNewFont("Funny", 14); 

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
	go_with_canvas.clear(); 
	
	return true;
}

Font ModuleUserInterface::GetFont(std::string font_name) const
{
	for (auto it = fonts_face_list.begin(); it != fonts_face_list.end(); it++)
	{		
		if ((*it)->name == font_name)
		{
			return *(*it);
		}			
	}

	return Font(); 
}

Font* ModuleUserInterface::LoadNewFont(std::string font_name, int size)
{
	Font* font_to_add = new Font();   

	font_to_add->name = font_name; 
	string path = App->file_system->GetFontsPath() + "\\" + font_name + ".ttf"; 
	FT_Error error = FT_New_Face(ft_library, path.c_str(), 0, &font_to_add->text_font);

	if (error)
	{
		CONSOLE_ERROR("An Error has occurred loading the font '%s'", font_name.c_str()); 
		return nullptr;
	}
	else
	{
		if (FT_HAS_VERTICAL(font_to_add->text_font))
		{
			CONSOLE_LOG("Vertical fonts not supported"); 
			return nullptr; 
		}

		FT_Set_Pixel_Sizes(font_to_add->text_font, 0, size);
		font_to_add->GenerateCharacterList();
		font_to_add->size = size; 
		fonts_face_list.push_back(font_to_add);
		return font_to_add;
	}

	FT_Done_Face(font_to_add->text_font);
	FT_Done_FreeType(ft_library);
}

void ModuleUserInterface::DeleteFont(std::string name)
{
	for (auto it = fonts_face_list.begin(); it != fonts_face_list.end(); it++)
	{
		if ((*it)->name == name)
		{
			fonts_face_list.erase(it); 
			return; 
		}
	}
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
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			
			ComponentRectTransform* rtransform = (ComponentRectTransform*)(*it)->GetComponent(CMP_RECTTRANSFORM);
			Transform canvas_transform = rtransform->GetTransform()->transform; 

			// args: left, right, bottom, top, near, far
			float left = canvas_transform .position.x -rtransform->width / 2;
			float right = canvas_transform.position.x + rtransform->width / 2;
			float bottom = canvas_transform.position.y -rtransform->height / 2;
			float top = canvas_transform.position.y + rtransform->height / 2;
			float near_plane = 1000.0f;
			float far_plane = -1000.0f;

			glOrtho(left, right, bottom, top, near_plane, far_plane);

			float3 min = {left, bottom, near_plane}; 
			float3 max = { right, top, far_plane};
		 
			ui_render_box.minPoint = min; 
			ui_render_box.maxPoint = max;

			App->renderer3D->UseDebugRenderSettings(); 			
		}
		
		//{
		//	LineSegment curr_line;

		//	glBegin(GL_LINES);
		//	App->renderer3D->UseDebugRenderSettings();
		//	glColor3f(1.0f, 0.0f, 0.0f);

		//	for (int i = 0; i < 12; i++)
		//	{
		//		curr_line = ui_render_box.Edge(i);

		//		glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
		//		glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		//	}

		//	glEnd();
		//}
	
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
			ComponentMesh* mesh_cmp = r_transform->GetRectQuadComponent(); 
			mesh_cmp->UpdateBoundingBox(r_transform->GetTransform());
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

AABB ModuleUserInterface::GetRenderBox() const
{
	return ui_render_box;
}
