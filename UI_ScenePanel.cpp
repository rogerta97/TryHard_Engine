#include "UI_ScenePanel.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "imgui_dock.h"

UI_ScenePanel::UI_ScenePanel()
{
	type = Panel_Types::CONFIGURATION_PANEL;
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
	if (ImGui::BeginDock("Scene",NULL,NULL,300))
	{
		//Get size of the window
		ImVec2 size = ImGui::GetContentRegionAvail();

		//Render the texture
		glEnable(GL_TEXTURE_2D);
		if(App->camera->GetViewportTexture() != nullptr)
		{
			ImGui::Image((void*)App->camera->GetViewportTexture()->GetTextureID(), size, ImVec2(0, 1), ImVec2(1, 0));

			
			//App->camera->GetViewportTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);
	
	}
	ImGui::EndDock();

	App->camera->GetViewportTexture()->Render();
	App->camera->GetViewportTexture()->Unbind();

	return true;
}

