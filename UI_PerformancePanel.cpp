#include "UI_PerformancePanel.h"
#include "imgui_dock.h"
#include "Application.h"


UI_PerformancePanel::UI_PerformancePanel()
{
}


UI_PerformancePanel::~UI_PerformancePanel()
{
}

bool UI_PerformancePanel::Start()
{
	show = false;

	return true;
}

bool UI_PerformancePanel::Update()
{
	if (ImGui::BeginDock("Performance", &show, NULL))
	{
		ImGui::Text("Init:"); ImGui::NewLine();

		ImGui::Text("Window Initialization: %d ms", App->window->init_time); ImGui::NewLine();
	}
	ImGui::EndDock();

	return true;
}
