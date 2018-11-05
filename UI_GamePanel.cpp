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
	show = true; 
	return true;
}

bool UI_GamePanel::Update()
{
	if (ImGui::Begin("Game", &show))
	{
		//Render the texture
		glEnable(GL_TEXTURE_2D);

		region_size = ImGui::GetContentRegionAvail();
		const float region_ratio = region_size.y / region_size.x;

		if (App->camera->GetGameCamera() != nullptr)
		{			
			Camera* camera = App->camera->GetGameCamera()->camera;
			pos = ImGui::GetWindowPos();
			camera->SetAspectRatio(camera->aspect_ratio / region_ratio);

			ImGui::Image((void*)App->camera->GetGameCamera()->GetViewportTexture()->GetTextureID(), region_size, ImVec2(0, 1), ImVec2(1, 0));

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
