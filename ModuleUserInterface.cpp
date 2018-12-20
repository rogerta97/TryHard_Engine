#include "ModuleUserInterface.h"
#include "UI_Image.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"
#include "ComponentCamera.h"
#include "SDL\include\SDL_events.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Application.h"
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
	
	LoadAllFonts(); 

	LoadNewFont("Antonio-Regular", 23); 

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

	//if (App->imgui->game_panel->is_mouse_inside)
	//{
	//	float2 norm_mouse_pos = //function that gets the position (0-1)
	//		//GetLastObjectWithCanvas
	//		//ComponentCanvas ----- Getcomponent(CMP_RectTransform)
	//		//GetPointFromCanvasPercentage(0.5f) -> 150 if with 300
	//		//mouse_pos = X

	//}

	return UPDATE_CONTINUE;
}

update_status ModuleUserInterface::PostUpdate(float dt)
{
	buttons_pressed.clear(); 
	return update_status::UPDATE_CONTINUE;
}


bool ModuleUserInterface::CleanUp()
{
	go_with_canvas.clear(); 
	
	return true;
}

void ModuleUserInterface::CleanCanvasList()
{
	go_with_canvas.clear(); 
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
		return nullptr;	
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

void ModuleUserInterface::LoadAllFonts()
{
	std::vector<std::string> font_files_name = std::vector<std::string>();
	App->file_system->GetFilesInDirectory(App->file_system->GetFontsPath().c_str(), font_files_name, false, false);

	for (auto it = font_files_name.begin(); it != font_files_name.end(); it++)
	{
		std::string name = App->file_system->DeleteFileExtension((*it).c_str());
		name = App->file_system->GetLastPathItem(name.c_str());
		LoadNewFont(name.c_str(), 25);
	}
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

void ModuleUserInterface::RecieveEvent(const Event & new_event)
{
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{
		(*it)->OnEvent(new_event);		
	}
}

void ModuleUserInterface::SendInput(SDL_Event * e)
{
	if (e->type == SDL_TEXTINPUT && *e->text.text != '8')
	{
		buttons_pressed.push_back(*e->text.text);
		return;
	}

	if (e->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
	{
		buttons_pressed.push_back(*e->text.text);
		return; 
	}
}

std::list<char>& ModuleUserInterface::GetInputLastFrame()
{
	return buttons_pressed; 
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
		}
		
		//App->renderer3D->UseDebugRenderSettings();
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
	bool add = true; 
	for (auto it = go_with_canvas.begin(); it != go_with_canvas.end(); it++)
	{
		if (canvas_go == (*it))
		{
			add = false;
			break; 
		}
		
	}

	if(add)
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

float3 ModuleUserInterface::GetMousePos() const
{
	return mouse_game_pos;
}

void ModuleUserInterface::SetMousePos(const float3 & new_pos)
{
	mouse_game_pos = new_pos;
}
