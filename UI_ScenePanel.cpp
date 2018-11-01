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
	name = "ScenePanel";
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
		
		region_size = ImGui::GetContentRegionAvail();
		const float region_ratio = region_size.y / region_size.x;

		Camera* camera = App->camera->GetEditorCamera()->camera;

		pos = ImGui::GetWindowPos();

		camera->SetAspectRatio(camera->aspect_ratio / region_ratio);

		ImGui::Image((void*)App->camera->GetEditorCamera()->GetViewportTexture()->GetTextureID(), region_size, ImVec2(0, 1), ImVec2(1, 0));
		
		glDisable(GL_TEXTURE_2D);

		is_mouse_in = ImGui::IsWindowHovered();

	
	}
	ImGui::End();

	App->camera->GetEditorCamera()->GetViewportTexture()->Render();
	App->camera->GetEditorCamera()->GetViewportTexture()->Unbind();


	return true;
}

ImVec2 UI_ScenePanel::GetMousePosInDock()
{
	ImVec2 mouse_pos, mouse_in_dock_pos;
	mouse_pos = ImGui::GetMousePos();

	int offset_x = 8; //the small margin between the begining of the dock and the begining of the image
	int offset_y = 28;
	mouse_in_dock_pos.x = mouse_pos.x - pos.x - offset_x;
	mouse_in_dock_pos.y = mouse_pos.y - pos.y - offset_y;

	return mouse_in_dock_pos;
}


ImVec2 UI_ScenePanel::GetMousePosInDockNormalized() //between -1 and 1
{
	ImVec2 mouse_in_dock_pos = GetMousePosInDock();
	ImVec2 mouse_normalized;

	mouse_normalized.x = mouse_in_dock_pos.x / region_size.x;
	mouse_normalized.y = mouse_in_dock_pos.y / region_size.y;

	mouse_normalized.x -= 0.5;
	mouse_normalized.x *= 2;

	mouse_normalized.y -= 0.5;
	mouse_normalized.y *= 2;

	mouse_normalized.y *= -1;

	//CONSOLE_LOG("x:%f, y:%f, w:%f, h:%f", mouse_normalized.x, mouse_normalized.y, region_size.x, region_size.y);
	return mouse_normalized;
}



