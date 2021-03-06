#include "UI_ConfigurationPanel.h"
#include "imgui_dock.h"
#include "Application.h"

UI_ConfigurationPanel::UI_ConfigurationPanel()
{
	type = Panel_Types::CONFIGURATION_PANEL;
	name = "Configuration";
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

	if (ImGui::Begin(name, &show ,NULL))
	{
		App->DisplayConfigData();

		for (int i = 0; App->GetModuleAt(i) != nullptr; i++)
		{
			App->GetModuleAt(i)->PrintConfigData();
		}
	}
	ImGui::End();  

	return true;
}
