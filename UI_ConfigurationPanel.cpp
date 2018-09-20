#include "UI_ConfigurationPanel.h"
#include "imgui_dock.h"
#include "Application.h"

UI_ConfigurationPanel::UI_ConfigurationPanel()
{
	type = Panel_Types::CONFIGURATION_PANEL;
}

UI_ConfigurationPanel::~UI_ConfigurationPanel()
{
}

bool UI_ConfigurationPanel::Start()
{
	show = true; 

	return true;
}

bool UI_ConfigurationPanel::Update()
{
	if (show == false)
		return false; 

	if (ImGui::BeginDock("Configuration",NULL,NULL))
	{
		App->DisplayConfigData();

		for (int i = 0; App->GetModuleAt(i) != nullptr; i++)
		{
			App->GetModuleAt(i)->PrintConfigData();
		}
	}
	ImGui::EndDock();  

	return true;
}
