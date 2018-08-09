#include "UI_ConsolePanel.h"
#include "Application.h"
#include "imgui_dock.h"
#include "Console.h"

#include <string>
#include <map>

UI_ConsolePanel::UI_ConsolePanel()
{
}


UI_ConsolePanel::~UI_ConsolePanel()
{
}

bool UI_ConsolePanel::Start()
{
	debug_color = ImVec4(0, 0, 1, 1); 
	log_color = ImVec4(1, 1, 1, 1);
	error_color = ImVec4(1, 0, 0, 1);

	show_debug = true; 
	show_log = true; 
	show_error = true; 

	show = true; 

	return false;
}

bool UI_ConsolePanel::Update()
{	
	if (ImGui::BeginDock("Console"))
	{
		std::string btn_name;

		//Debug Button
		btn_name = "Debug(" + std::to_string(App->imgui->console.GetDebugCount()) + ")"; 
		if (ImGui::Button(btn_name.c_str()))
		{
			show_debug = !show_debug; 
		}
		ImGui::SameLine(); 

		//Log Button
		btn_name = "Log(" + std::to_string(App->imgui->console.GetLogCount()) + ")";
		if (ImGui::Button(btn_name.c_str()))
		{
			show_log = !show_log;
		}
		ImGui::SameLine();

		//Errors Button
		btn_name = "Error(" + std::to_string(App->imgui->console.GetErrorCount()) + ")";
		if (ImGui::Button(btn_name.c_str()))
		{
			show_error = !show_error;
		}
		
		if (App->imgui->console.GetBuffer().size() == 0)
			return true;

		for (auto it = App->imgui->console.console_buffer.begin(); it != App->imgui->console.console_buffer.end(); it++)
		{
			switch (it->first)
			{
			case Console_Message_Type::CONSOLE_MLOG:
				if(show_log)
					ImGui::TextColored(log_color, it->second);
				break;

			case Console_Message_Type::CONSOLE_MDEBUG:
				if (show_debug)
					ImGui::TextColored(debug_color, it->second);
				break;

			case Console_Message_Type::CONSOLE_MERROR:
				if (show_error)
					ImGui::TextColored(error_color, it->second);
				break;
			}
		}
	}
	ImGui::EndDock();



	return false;
}
