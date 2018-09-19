#include "UI_RandomNumberPanel.h"
#include "Application.h"
#include "imgui_dock.h"

UI_RandomNumberPanel::UI_RandomNumberPanel()
{
}


UI_RandomNumberPanel::~UI_RandomNumberPanel()
{
}

bool UI_RandomNumberPanel::Start()
{
	generated_num = 0; 
	show = false; 

	return true;
}

bool UI_RandomNumberPanel::Update()
{
	if (show == false)
		return true; 

	if (ImGui::BeginDock("Random Generator", NULL, NULL))
	{
		ImGui::Text("Im working"); 
	}

	ImGui::EndDock();

	return true;
}
