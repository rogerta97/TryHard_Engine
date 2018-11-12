#include "UI_ScenePanel.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "imgui_dock.h"
#include "ImGuizmo\ImGuizmo.h"

#include "Material.h"

UI_ScenePanel::UI_ScenePanel()
{
	type = Panel_Types::SCENE_PANEL;
	play = false;
	pause = false;
}

UI_ScenePanel::~UI_ScenePanel()
{
	name = "ScenePanel";
}

bool UI_ScenePanel::Start()
{
	show = true;
	is_mouse_in = false;

	play_icon = (Material*)App->resources->Get(RES_MATERIAL, "PlayIcon");
	play_icon->LoadToMemory(); 
	play_icon->reference_counting++; 

	pause_icon = (Material*)App->resources->Get(RES_MATERIAL, "PauseIcon");
	pause_icon->LoadToMemory();
	pause_icon->reference_counting++;

	stop_icon = (Material*)App->resources->Get(RES_MATERIAL, "StopIcon");
	stop_icon->LoadToMemory();
	stop_icon->reference_counting++;


	return true;
}

bool UI_ScenePanel::Update()
{
	ImGuiWindowFlags flags = NULL;
	if (ImGuizmo::IsOver())
		flags = flags | ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Scene", &show, flags))
	{

		region_size = ImGui::GetContentRegionAvail();


		ImGui::BeginChild("SceneView");

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

		App->scene->DrawGuizmo();

		//Buttons
		ImGui::SetCursorPos({ region_size.x / 2 - 30,5 });
		if (play_icon)
			ImGui::ImageButton((ImTextureID)play_icon->GetDiffuseTexture()->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 3);

		if (ImGui::IsItemClicked())
		{
			play = !play;
			App->BroadCastEvent(Event(Event::PLAY));
		}

		//if (play)
		//	ImGui::set

		ImGui::SameLine();

		if (stop_icon)
			ImGui::ImageButton((ImTextureID)stop_icon->GetDiffuseTexture()->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 3);

		if (ImGui::IsItemClicked())
		{

		}

		ImGui::SameLine();

		if (pause_icon)
			ImGui::ImageButton((ImTextureID)pause_icon->GetDiffuseTexture()->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 3);

		if (ImGui::IsItemClicked())
		{
			App->BroadCastEvent(Event(Event::PAUSE));
		}
		ImGui::SetCursorPos({ 0,0 });
	}

	ImGui::EndChild();
	ImGui::End();

	App->camera->GetEditorCamera()->GetViewportTexture()->Render();
	App->camera->GetEditorCamera()->GetViewportTexture()->Unbind();

	return true;
}

ImVec2 UI_ScenePanel::GetMousePosInDock()
{
	ImVec2 mouse_pos, mouse_in_dock_pos;
	mouse_pos = ImGui::GetMousePos();

	int offset_x = 0; //the small margin between the begining of the dock and the begining of the image
	int offset_y = 0;
	mouse_in_dock_pos.x = mouse_pos.x - pos.x - offset_x;
	mouse_in_dock_pos.y = mouse_pos.y - pos.y - offset_y;

	//CONSOLE_LOG("x:%f, y:%f, w:%f, h:%f", mouse_in_dock_pos.x, mouse_in_dock_pos.y, region_size.x, region_size.y);

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


	return mouse_normalized;
}

ImVec2 UI_ScenePanel::GetPos() const
{
	return pos;
}

ImVec2 UI_ScenePanel::GetSize() const
{
	return region_size;
}



