#include "UI_GamePanel.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "imgui_dock.h"

#include "MaterialImporter.h"


UI_GamePanel::UI_GamePanel()
{
	name = "GamePanel"; 
	game_size.x = 0;
	game_size.y = 0;
	size_changed = false;
	margin_type = NONE;
}


UI_GamePanel::~UI_GamePanel()
{
}

bool UI_GamePanel::Start()
{
	ar = STANDARD; //mmgr assert trigerred when uncommented wtf?
	game_ar = 0.5625f;
	show = true; 
	return true;
	is_mouse_in = false;
}

bool UI_GamePanel::Update()
{
	ImGuiWindowFlags flags = NULL;
	flags = flags | ImGuiWindowFlags_NoScrollWithMouse;
	flags = flags | ImGuiWindowFlags_NoScrollbar;

	if (ImGui::Begin("Game", &show,flags))
	{
		//Render the texture
		glEnable(GL_TEXTURE_2D);

		is_mouse_in = ImGui::IsWindowHovered();

		region_size = ImGui::GetWindowSize();

		region_size.y -= 25; //The height of the top bar

		if (App->camera->GetGameCamera() != nullptr)
		{			
			Camera* camera = App->camera->GetGameCamera()->camera;
			pos = ImGui::GetWindowPos();

			ImVec2 size = CalculateSizeAndSetCursor(game_ar);

			camera->SetAspectRatio(camera->aspect_ratio / game_ar);

			if (game_size.x != size.x || game_size.y != size.y)
			{
				size_changed = true;
			}			
			else
				size_changed = false;

			game_size.y = size.y;
			game_size.x = size.x;

			if (size_changed)
			{
				App->user_interface->AddaptCanvasToScreen();
				size_changed = false;
			}
				

			ImGui::Image((void*)App->camera->GetGameCamera()->GetViewportTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));

			App->camera->GetGameCamera()->GetViewportTexture()->Render();
			App->camera->GetGameCamera()->GetViewportTexture()->Unbind();

			ImGui::SetCursorPosY(25);
			ShowARSelector();
		}
		else
		{
			ImGui::Image((void*)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), region_size, ImVec2(0, 1), ImVec2(1, 0));
		}
		
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::End();
	return true;

}

bool UI_GamePanel::CleanUp()
{
	return true;
}

ImVec2 UI_GamePanel::CalculateSizeAndSetCursor(float original_aspect_ratio)
{
	ImVec2 size = { 0,0 };

	const float region_ratio = region_size.y / region_size.x;

	float offset_x = 0;
	float offset_y = 21;

	float size_offset = 0.9;

	ImGui::SetCursorPos({ offset_x,offset_y });

	float difference = 0;
	if (region_ratio < original_aspect_ratio)
	{
		size.y = region_size.y;
		size.x = region_size.y / original_aspect_ratio;

		difference = region_size.x - size.x;

		game_tex_pos = { difference / 2 , 0 };

		ImGui::SetCursorPosX(game_tex_pos.x);

		margin_type = HORIZONTAL;
	}
	else if (region_ratio > original_aspect_ratio)
	{
		size.x = region_size.x;
		size.y = region_size.x * original_aspect_ratio;

		difference = region_size.y - size.y;

		game_tex_pos = { 0, (difference / 2) + offset_y};

		ImGui::SetCursorPosY(game_tex_pos.y);
		margin_type = VERTICAL;
	}
	else
	{
		size.x = region_size.x;
		size.y = region_size.x;
		margin_type = NONE;
	}

	return size;
}

void UI_GamePanel::ShowARSelector() //WIP, not used yet
{
	int selected = ar;
	ImGui::PushItemWidth(100);
	if (ImGui::Combo("Aspect ratio", &selected, " 16:9\0 1:1\0Windowed"))
	{
		ar = (ar_type)selected;
	}

	switch (ar)
	{
	case 0:
		game_ar = 0.5625f;
		break;

	case 1:
		game_ar = 1.0f;
		break;

	case 2:
		game_ar = 0.5625f;
		break;
	}
}

ImVec2 UI_GamePanel::GetGameTexSize() const
{
	return game_size;
}

ImVec2 UI_GamePanel::GetMousePosInDock()
{
	ImVec2 mouse_pos, mouse_in_dock_pos;
	mouse_pos = ImGui::GetMousePos();

	int offset_x = game_tex_pos.x; 
	int offset_y = game_tex_pos.y;

	mouse_in_dock_pos.x = mouse_pos.x - pos.x - offset_x;
	mouse_in_dock_pos.y = mouse_pos.y - pos.y - offset_y;

	switch (margin_type)
	{
	case NONE:
		break;
	case VERTICAL:
		mouse_in_dock_pos.x -= 4;
		game_size.x -= 12;
		break;
	case HORIZONTAL:
		mouse_in_dock_pos.y -= 20;
		break;
	default:
		break;
	}

	//CONSOLE_LOG("x:%f, y:%f, w:%f, h:%f", mouse_in_dock_pos.x, mouse_in_dock_pos.y, game_size.x, game_size.y);

	return mouse_in_dock_pos;
}

ImVec2 UI_GamePanel::GetMousePosInDockNormalized()
{
	ImVec2 mouse_in_dock_pos = GetMousePosInDock();
	ImVec2 mouse_normalized;

	mouse_normalized.x = mouse_in_dock_pos.x / game_size.x;
	mouse_normalized.y = mouse_in_dock_pos.y / game_size.y;

	mouse_normalized.x -= 0.5;
	mouse_normalized.x *= 2;

	mouse_normalized.y -= 0.5;
	mouse_normalized.y *= 2;

	mouse_normalized.y *= -1;

	//CONSOLE_LOG("x:%f, y:%f", mouse_normalized.x, mouse_normalized.y);

	return mouse_normalized;
}

ImVec2 UI_GamePanel::GetMousePosInDockZeroOne()
{
	ImVec2 mouse_in_dock_pos = GetMousePosInDock();
	ImVec2 mouse_normalized;

	mouse_normalized.x = mouse_in_dock_pos.x / game_size.x;
	mouse_normalized.y = mouse_in_dock_pos.y / game_size.y;

	mouse_normalized.y = 1 - mouse_normalized.y;

	CONSOLE_LOG("x:%f, y:%f", mouse_normalized.x, mouse_normalized.y);

	return mouse_normalized;
}
