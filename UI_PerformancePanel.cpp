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
	return true;
}

bool UI_PerformancePanel::Update()
{
	if (ImGui::Begin("Performance", &show))
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 5;

		const int histogram_height = 130;
		list_modules = *App->getModuleList();
		module_iterator = list_modules.begin();

		size = ImGui::GetContentRegionAvail();
		size.x = size.x / 2;

		//INIT
		if (ImGui::CollapsingHeader("Init times")) 
		{
			std::vector<float>	init_time_buffer;

			ImGui::Columns(2, "", false);

			while (module_iterator != list_modules.end())
			{
				ImGui::BulletText("%s: %d ms", (*module_iterator)->name, (*module_iterator)->init_time);
				init_time_buffer.push_back((*module_iterator)->init_time);
				module_iterator++;
			}

			ImGui::NextColumn();
			ImGui::PlotHistogram("##Framerate", &init_time_buffer[0], init_time_buffer.size(), 0, "Init times in order", 0.0f, 1000.0f, ImVec2(size.x, histogram_height));

			ImGui::Columns(1);
			ImGui::Separator();

		}

		//STARTS
		module_iterator = list_modules.begin();

		if (ImGui::CollapsingHeader("Start times"))
		{
			std::vector<float>	start_time_buffer;

			ImGui::Columns(2, "", false);

			while (module_iterator != list_modules.end())
			{
				ImGui::BulletText("%s: %d ms", (*module_iterator)->name, (*module_iterator)->start_time);
				start_time_buffer.push_back((*module_iterator)->start_time);
				module_iterator++;
			}

			ImGui::NextColumn();
			ImGui::PlotHistogram("##Framerate", &start_time_buffer[0], start_time_buffer.size(), 0, "Start times in order", 0.0f, 1000.0f, ImVec2(size.x, histogram_height));
			ImGui::Columns(1);
			ImGui::Separator();
		}

		//RUNTIME
		PrintPreUpdatePlots();
		PrintUpdatePlots();
		PrintPostUpdatePlots();

	}
	ImGui::End();

	return true;
}

void UI_PerformancePanel::PrintPreUpdatePlots()
{
	if (ImGui::CollapsingHeader("Preupdate data"))
	{
		module_iterator = list_modules.begin();

		std::vector<float>	runtimes_buffer;

		ImGui::Columns(2, "", false);
		while (module_iterator != list_modules.end())
		{
			if (!(*module_iterator)->preupdate_ms_buffer.empty()) {
				ImGui::PlotLines("", &(*module_iterator)->preupdate_ms_buffer[0], (*module_iterator)->preupdate_ms_buffer.size(), 0, (*module_iterator)->name, 0.0f, 20.0f, ImVec2(size.x, 100));
				ImGui::NextColumn();

				runtimes_buffer.push_back((*module_iterator)->preupdate_ms_buffer[0]);
			}
			module_iterator++;
		}

		ImGui::Columns(1);
		ImGui::NewLine();
		ImGui::PlotHistogram("##Framerate", &runtimes_buffer[0], runtimes_buffer.size(), 0, "Runtimes of all modules in order", 0.0f, 10.0f, ImVec2(size.x * 2, 200));
		runtimes_buffer.clear();
	}
}

void UI_PerformancePanel::PrintUpdatePlots()
{
	if (ImGui::CollapsingHeader("Update data"))
	{
		module_iterator = list_modules.begin();

		std::vector<float>	runtimes_buffer;

		ImGui::Columns(2, "", false);
		while (module_iterator != list_modules.end())
		{
			if (!(*module_iterator)->update_ms_buffer.empty()) {
				ImGui::PlotLines("", &(*module_iterator)->update_ms_buffer[0], (*module_iterator)->update_ms_buffer.size(), 0, (*module_iterator)->name, 0.0f, 20.0f, ImVec2(size.x, 100));
				ImGui::NextColumn();

				runtimes_buffer.push_back((*module_iterator)->update_ms_buffer[0]);
			}
			module_iterator++;
		}

		ImGui::Columns(1);
		ImGui::NewLine();
		ImGui::PlotHistogram("##Framerate", &runtimes_buffer[0], runtimes_buffer.size(), 0, "Runtimes of all modules in order", 0.0f, 10.0f, ImVec2(size.x * 2, 200));
		runtimes_buffer.clear();
	}
}

void UI_PerformancePanel::PrintPostUpdatePlots()
{

	if (ImGui::CollapsingHeader("Post Update data"))
	{
		module_iterator = list_modules.begin();

		std::vector<float>	runtimes_buffer;

		ImGui::Columns(2, "", false);
		while (module_iterator != list_modules.end())
		{
			if (!(*module_iterator)->preupdate_ms_buffer.empty()) {
				ImGui::PlotLines("", &(*module_iterator)->preupdate_ms_buffer[0], (*module_iterator)->preupdate_ms_buffer.size(), 0, (*module_iterator)->name, 0.0f, 20.0f, ImVec2(size.x, 100));
				ImGui::NextColumn();

				runtimes_buffer.push_back((*module_iterator)->preupdate_ms_buffer[0]);
			}
			module_iterator++;
		}

		ImGui::Columns(1);
		ImGui::NewLine();
		ImGui::PlotHistogram("##Framerate", &runtimes_buffer[0], runtimes_buffer.size(), 0, "Runtimes of all modules in order", 0.0f, 10.0f, ImVec2(size.x * 2, 200));
		runtimes_buffer.clear();
	}
}
