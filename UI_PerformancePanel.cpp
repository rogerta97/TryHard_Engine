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

		ImVec2 size = ImGui::GetContentRegionAvail();

		std::vector<float>	init_time_buffer;

		while (item != list_modules.end())
		{
			ImGui::Text("%s Initialization: %d ms", (*item)->name, (*item)->init_time); ImGui::NewLine();
			init_time_buffer.push_back((*item)->init_time);
			item++;
		}

		ImGui::PlotHistogram("##Framerate", &init_time_buffer[0], init_time_buffer.size(), 0, "Init times in order", 0.0f, 1000.0f, ImVec2(size.x, 100));


		//STARTS
		item = list_modules.begin();

		std::vector<float>	start_time_buffer;

		while (item != list_modules.end())
		{
			ImGui::Text("%s Start time: %d ms", (*item)->name, (*item)->start_time); ImGui::NewLine();
			start_time_buffer.push_back((*item)->start_time);
			item++;
		}

		ImGui::PlotHistogram("##Framerate", &start_time_buffer[0], start_time_buffer.size(), 0, "Start times in order", 0.0f, 1000.0f, ImVec2(size.x, 100));

		//RUNTIME
		item = list_modules.begin();

		while (item != list_modules.end())
		{
			if (!(*item)->runtime_ms_buffer.empty())
				ImGui::PlotLines("", &(*item)->runtime_ms_buffer[0], (*item)->runtime_ms_buffer.size(), 0, (*item)->name, 0.0f, 20.0f, ImVec2(300, 100));

			item++;
		}

	}
	ImGui::EndDock();

	return true;
}
