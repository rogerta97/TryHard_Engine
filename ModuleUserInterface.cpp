#include "ModuleUserInterface.h"
#include "UI_Image.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"
#include "ModuleImGui.h"
#include "UI_GamePanel.h"
#include "ComponentCamera.h"
#include "SDL\include\SDL_events.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "OpenGL.h"
#include "DebugDraw.h"
#include "Font.h"
#include "ComponentCanvas.h"
#include "ComponentButton.h"
#include "UI_Canvas.h"
#include "UI_Button.h"

#include "mmgr\mmgr.h"


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

	interpolating = false; 

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

	if (App->imgui->game_panel->is_mouse_in)
	{
		float2 norm_mouse_pos = App->imgui->game_panel->GetMousePosInDockZeroOne();
		GameObject* canvas_go = GetLastCanvas();

		if (canvas_go == nullptr)
			return UPDATE_CONTINUE; 

		ComponentRectTransform* canvas_rect_trans = (ComponentRectTransform*)canvas_go->GetComponent(CMP_RECTTRANSFORM);
		float2 mouse_pos_in_canvas = float2(canvas_rect_trans->GetPointFromPercentage(norm_mouse_pos).x, canvas_rect_trans->GetPointFromPercentage(norm_mouse_pos).y);


		//CONSOLE_LOG("x:%f, y:%f", mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
		ComponentCanvas* cmp_canvas = (ComponentCanvas*)canvas_go->GetComponent(CMP_CANVAS);

		std::vector<GameObject*> intersected_elements;

		UI_Canvas* ui_canvas = cmp_canvas->GetCanvas();
		ui_canvas->elements_in_canvas;

		for (auto ui_iterator = ui_canvas->elements_in_canvas.begin(); ui_iterator != ui_canvas->elements_in_canvas.end(); ui_iterator++)
		{
			ComponentRectTransform* elem_rect = (ComponentRectTransform*)(*ui_iterator)->GetComponent(CMP_RECTTRANSFORM);

			bool inside = elem_rect->isMouseInsideRect(mouse_pos_in_canvas);

			if (inside && (*ui_iterator)->GetComponent(CMP_BUTTON))			
				intersected_elements.push_back((*ui_iterator));
		}

		//auto last_list_item = intersected_elements.back();

		GameObject* element_on_top = nullptr;

		if (intersected_elements.size() > 0)
			element_on_top = intersected_elements.back();

		if (element_on_top)
		{
			//CONSOLE_LOG(element_on_top->name.c_str());
			ComponentButton* cmp_button = (ComponentButton*)element_on_top->GetComponent(CMP_BUTTON);
			UI_Button* button = cmp_button->GetButton();

			if (element_on_top->GetComponent(CMP_BUTTON))
			{
				if (cmp_button->has_mouse_entered == false)
				{
					Event new_event;
					new_event.type = UI_ELEMENT_ENTER;
					new_event.button.but = button;

					App->BroadCastEvent(new_event);
					cmp_button->has_mouse_entered = true; 
				}

				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					Event new_event; 
					new_event.type = UI_ELEMENT_DOWN;
					new_event.button.but = button; 

					App->camera->SetLocked(true); 
					
					App->BroadCastEvent(new_event); 
				}
				else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					Event new_event;
					new_event.type = UI_ELEMENT_UP;
					new_event.button.but = button;

					App->BroadCastEvent(new_event);
				}
			}
		}

		if (intersected_elements.size() < last_intersected_elements.size()) //this mean some gameobject is not under the mouse any more 
		{
			int max_size = last_intersected_elements.size(); 

			for (int i = intersected_elements.size() <= 0 ? 0: intersected_elements.size() - 1; i < last_intersected_elements.size(); i++)
			{
				ComponentButton* cmp_button = (ComponentButton*)last_intersected_elements[i]->GetComponent(CMP_BUTTON);
				
				if (cmp_button)
				{
					Event new_event;
					new_event.type = UI_ELEMENT_OUT;
					new_event.button.but = cmp_button->GetButton();

					cmp_button->has_mouse_entered = false;

					App->BroadCastEvent(new_event);
				}
			
			}	
		}

		last_intersected_elements = intersected_elements;

	}

	//Alpha interpolation
	InterpolateAlpha(); 

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
			(*it)->CleanCharacterList();
			delete (*it); 
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
			float left = canvas_transform.position.x - rtransform->width / 2;
			float right = canvas_transform.position.x + rtransform->width / 2;
			float bottom = canvas_transform.position.y - rtransform->height / 2;
			float top = canvas_transform.position.y + rtransform->height / 2;
			float near_plane = 1000.0f;
			float far_plane = -1000.0f;

			glOrtho(left, right, bottom, top, near_plane, far_plane);

			float3 min = { left, bottom, near_plane };
			float3 max = { right, top, far_plane };

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

	if (add)
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

void ModuleUserInterface::InterpolateAlpha()
{
	if (interpolating)
	{
		float alpha_percentage = (interpolation_timer.Read() / (interpolate_in*1000)); 

		if (alpha_percentage >= 1.0f)
		{
			finished_interpolation = true;
			interpolating = false; 
		}

		Event alpha_event;

		alpha_event.type = INTERPOLATE_ALPHA; 
		alpha_event.alpha_lvl.percentage = (1 - alpha_percentage); 

		App->BroadCastEvent(alpha_event);
	}

}

void ModuleUserInterface::SetInterpolation(bool value, float time)
{
	interpolating = value;
	interpolate_in = time; 
	finished_interpolation = false; 
	interpolation_timer.Start();
}

bool ModuleUserInterface::IsInterpolating()
{
	return interpolating; 
}

bool ModuleUserInterface::HasInterpolationEnded()
{
	return finished_interpolation;
}
