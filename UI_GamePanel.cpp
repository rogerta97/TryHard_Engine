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
		ImGui::SetCursorPosX(difference / 2);
	}
	else if (region_ratio > original_aspect_ratio)
	{
		size.x = region_size.x;
		size.y = region_size.x * original_aspect_ratio;

		difference = region_size.y - size.y;
		ImGui::SetCursorPosY((difference / 2) + offset_y);
	}
	else
	{
		size.x = region_size.x;
		size.y = region_size.x;
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
