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

		//Get size of the window
		ImVec2 size = ImGui::GetContentRegionAvail();

		float difference = 0;

		if (size.x < size.y)
		{
			difference = size.y - size.x;
			size.y = size.x;
		}
		else if (size.y < size.x) 
		{
			difference = size.x - size.y;
			size.x = size.y;
		}

		ImGui::SetCursorPosX(difference / 2);

		//Render the texture
		glEnable(GL_TEXTURE_2D);
		if(App->camera->GetViewportTexture() != nullptr)
		{

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

