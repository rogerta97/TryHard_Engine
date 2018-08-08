#include "UI_ConfigurationPanel.h"
#include "imgui_dock.h"
#include "Application.h"

UI_ConfigurationPanel::UI_ConfigurationPanel()
{
}

UI_ConfigurationPanel::~UI_ConfigurationPanel()
{
}

bool UI_ConfigurationPanel::Start()
{
	show = false; 

	return true;
}

bool UI_ConfigurationPanel::Update()
{
	if (show == false)
		return false; 

	if (ImGui::BeginDock("Configuration"))
	{
		if (ImGui::CollapsingHeader("HardWare"))
		{
			SDL_version version;
			SDL_GetVersion(&version);

			ImGui::Text("SDL Version"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			float tmp_speed = App->camera->GetSpeed();
			ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
			App->camera->SetSpeed(tmp_speed);
		}
	}
	ImGui::EndDock();  

	return true;
}
