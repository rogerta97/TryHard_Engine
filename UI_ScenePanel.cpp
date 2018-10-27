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
	is_mouse_in = false;
	return true;
}

bool UI_ScenePanel::Update()
{
	if (ImGui::Begin("Scene", &show))
	{
		//Render the texture
		glEnable(GL_TEXTURE_2D);
		if(App->camera->GetEditorCamera()->GetViewportTexture() != nullptr)
		{
			ImVec2 region_size = ImGui::GetContentRegionAvail();
			const float region_ratio = region_size.y / region_size.x;

			Camera* camera = App->camera->GetEditorCamera()->camera;


			camera->SetAspectRatio(camera->aspect_ratio / region_ratio);

			ImGui::Image((void*)App->camera->GetEditorCamera()->GetViewportTexture()->GetTextureID(), region_size, ImVec2(0, 1), ImVec2(1, 0));

			//App->camera->GetViewportTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);

		is_mouse_in = ImGui::IsWindowHovered();

	
	}
	ImGui::End();

	App->camera->GetEditorCamera()->GetViewportTexture()->Render();
	App->camera->GetEditorCamera()->GetViewportTexture()->Unbind();

	return true;
}

