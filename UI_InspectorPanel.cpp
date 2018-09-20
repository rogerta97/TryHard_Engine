#include "UI_InspectorPanel.h"

#include "Application.h"
#include "imgui_dock.h"


UI_InspectorPanel::UI_InspectorPanel()
{
}


UI_InspectorPanel::~UI_InspectorPanel()
{
}

bool UI_InspectorPanel::Start()
{
	show = true;

	return true;
}

bool UI_InspectorPanel::Update()
{
	if (show == false)
		return false; 

	if (ImGui::BeginDock("Inspector", &show, NULL))
	{
		ImGui::Text("No GameObject Selected."); 
	}

	ImGui::EndDock();

	return true;
}

void UI_InspectorPanel::SetGameObject(GameObject * new_go)
{
	gameobject = new_go; 
}

GameObject * UI_InspectorPanel::GetGameObject()
{
	return gameobject;
}
