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
		std::list<Module*> list_modules = *App->getModuleList();
		std::list<Module*>::iterator item = list_modules.begin();

		ImVec2 size = ImGui::GetContentRegionAvail();
		size.x = size.x / 2;

		//INIT
		if (ImGui::CollapsingHeader("Init times")) 
		{
			std::vector<float>	init_time_buffer;

			ImGui::Columns(2, "", false);

			while (item != list_modules.end())
			{
				ImGui::BulletText("%s: %d ms", (*item)->name, (*item)->init_time);
				init_time_buffer.push_back((*item)->init_time);
				item++;
			}

			ImGui::NextColumn();
			ImGui::PlotHistogram("##Framerate", &init_time_buffer[0], init_time_buffer.size(), 0, "Init times in order", 0.0f, 1000.0f, ImVec2(size.x, 100));

			ImGui::Columns(1);
			ImGui::Separator();

		}

		//STARTS
		item = list_modules.begin();

		if (ImGui::CollapsingHeader("Start times"))
		{
			std::vector<float>	start_time_buffer;

			ImGui::Columns(2, "", false);

			while (item != list_modules.end())
			{
				ImGui::BulletText("%s: %d ms", (*item)->name, (*item)->start_time);
				start_time_buffer.push_back((*item)->start_time);
				item++;
			}

			ImGui::NextColumn();
			ImGui::PlotHistogram("##Framerate", &start_time_buffer[0], start_time_buffer.size(), 0, "Start times in order", 0.0f, 1000.0f, ImVec2(size.x, 100));
			ImGui::Columns(1);
			ImGui::Separator();
		}

		//RUNTIME
		if (ImGui::CollapsingHeader("Runtime data"))
		{
			item = list_modules.begin();

			std::vector<float>	runtimes_buffer;

			ImGui::Columns(2, "", false);
			while (item != list_modules.end())
			{
				if (!(*item)->runtime_ms_buffer.empty()) {
					ImGui::PlotLines("", &(*item)->runtime_ms_buffer[0], (*item)->runtime_ms_buffer.size(), 0, (*item)->name, 0.0f, 20.0f, ImVec2(size.x, 100));
					ImGui::NextColumn();

					runtimes_buffer.push_back((*item)->runtime_ms_buffer[0]);
				}
				item++;
			}

			ImGui::Columns(1);
			ImGui::NewLine();
			ImGui::PlotHistogram("##Framerate", &runtimes_buffer[0], runtimes_buffer.size(), 0, "Runtimes of all modules in order", 0.0f, 10.0f, ImVec2(size.x * 2, 200));
			runtimes_buffer.clear();
		}
	}
	ImGui::EndDock();

	return true;
}
