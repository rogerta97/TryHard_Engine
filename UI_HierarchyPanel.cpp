#include "UI_HierarchyPanel.h"
#include "imgui_dock.h"


UI_HierarchyPanel::UI_HierarchyPanel()
{
}


UI_HierarchyPanel::~UI_HierarchyPanel()
{
}

bool UI_HierarchyPanel::Start()
{
	show = true; 

	return true;
}

bool UI_HierarchyPanel::Update()
{
	if (ImGui::BeginDock("Hierarchy", &show, NULL))
	{
		ImGui::Text("Coloca'm pooooleteee");

		
	}

	ImGui::EndDock(); 

	return true;
}

bool UI_HierarchyPanel::CleanUp()
{
	return true;
}
