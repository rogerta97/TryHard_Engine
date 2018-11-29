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
}


UI_GamePanel::~UI_GamePanel()
{
}

bool UI_GamePanel::Start()
{
	ar = STANDARD; //mmgr assert trigerred when uncommented wtf?
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

		region_size.y -= 25;

		float original_tex_ratio = ShowARSelector();//0.5625f;// ShowARSelector();

		if (App->camera->GetGameCamera() != nullptr)
		{			
			Camera* camera = App->camera->GetGameCamera()->camera;
			pos = ImGui::GetWindowPos();

			ImVec2 size = CalculateSizeAndSetCursor(original_tex_ratio);

			camera->SetAspectRatio(camera->aspect_ratio / original_tex_ratio);



			ImGui::Image((void*)App->camera->GetGameCamera()->GetViewportTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));

			App->camera->GetGameCamera()->GetViewportTexture()->Render();
			App->camera->GetGameCamera()->GetViewportTexture()->Unbind();
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

	return size;
}

float UI_GamePanel::ShowARSelector() //WIP, not used yet
{
	float new_ar = 0.5625f;

	int selected = ar;

	if (ImGui::Combo("Aspect ratio", &selected, " 16:9\0 1:1\0Windowed"))
	{
		Uint32 flags;

		switch (selected)
		{
		case 0:
			new_ar = 0.5625f;
			break;

		case 1:
			new_ar = 1.0f;
			break;

		case 2:
			new_ar = 0.5625f;
			break;
		}

		ar = (ar_type)selected;
	}

	switch (ar)
	{
	case 0:
		new_ar = 0.5625f;
		break;

	case 1:
		new_ar = 1.0f;
		break;

	case 2:
		new_ar = 0.5625f;
		break;
	}

	return new_ar;
}
