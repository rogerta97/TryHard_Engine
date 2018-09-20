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

		std::list<Module*> list_modules = *App->getModuleList();

		std::list<Module*>::iterator item = list_modules.begin();

		std::vector<float>	init_time_buffer;

		while (item != list_modules.end())
		{
			ImGui::Text("%s Initialization: %d ms", (*item)->name, (*item)->init_time); ImGui::NewLine();
			init_time_buffer.push_back((*item)->init_time);
			item++;
		}

		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::PlotHistogram("##Framerate", &init_time_buffer[0], init_time_buffer.size(), 0, "Init times in order", 0.0f, 150.0f, ImVec2(size.x, 100));
	}
	ImGui::EndDock();

	return true;
}
