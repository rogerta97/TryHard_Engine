#include "UI_ScenePanel.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "imgui_dock.h"

UI_ScenePanel::UI_ScenePanel()
{
	type = Panel_Types::SCENE_PANEL;
}

UI_ScenePanel::~UI_ScenePanel()
{
}

bool UI_ScenePanel::Start()
{
	show = true;
	return true;
}

bool UI_ScenePanel::Update()
{
	if (ImGui::Begin("Scene", &show))
	{
		//Render the texture
		glEnable(GL_TEXTURE_2D);
		if(App->camera->GetViewportTexture() != nullptr)
		{

			ImVec2 size = { 0,0 };
			ImVec2 region_size = ImGui::GetContentRegionAvail();
			const float region_ratio = region_size.y / region_size.x;

			uint original_tex_width = App->camera->GetViewportTexture()->GetWidth();
			float original_tex_height = App->camera->GetViewportTexture()->GetHeight();

			float original_tex_ratio = original_tex_height / original_tex_width;
			float difference = 0;

			if (region_ratio < original_tex_ratio)
			{
				size.y = region_size.y;
				size.x = region_size.y / original_tex_ratio;

				difference = region_size.x - size.x;
				ImGui::SetCursorPosX(difference / 2);
			}
			else if (region_ratio > original_tex_ratio)
			{
				size.x = region_size.x;
				size.y = region_size.x * original_tex_ratio;

				difference = region_size.y - size.y;
				ImGui::SetCursorPosY(difference / 2);
			}


			ImGui::Image((void*)App->camera->GetViewportTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));	

			//App->camera->GetViewportTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	
	}
	ImGui::End();

	App->camera->GetViewportTexture()->Render();
	App->camera->GetViewportTexture()->Unbind();

	return true;
}

