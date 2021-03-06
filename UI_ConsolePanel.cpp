#include "UI_ConsolePanel.h"
#include "Application.h"
#include "imgui_dock.h"
#include "Console.h"

#include <string>
#include <map>

UI_ConsolePanel::UI_ConsolePanel()
{
	name = "Console";
}


UI_ConsolePanel::~UI_ConsolePanel()
{
	type = CONSOLE_PANEL;
}

bool UI_ConsolePanel::Start()
{
	debug_color = ImVec4(0.2, 0.2, 1, 1); 
	log_color = ImVec4(1, 1, 1, 1);
	error_color = ImVec4(1, 0.1, 0.1, 1);

	show_debug = true; 
	show_log = true; 
	show_error = true; 

	show = true; 

	return false;
}

bool UI_ConsolePanel::Update()
{	
	if (ImGui::Begin(name, &show,NULL))
	{
		std::string btn_name;

		//Debug Button
		btn_name = "Debug(" + std::to_string(App->imgui->console.GetDebugCount()) + ")"; 
		if (ImGui::Button(btn_name.c_str()))
		{
			show_debug = !show_debug; 
		}
		ImGui::SameLine(); 

		//CONSOLE_LOG Button

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.4f));

		btn_name = "Log(" + std::to_string(App->imgui->console.GetLogCount()) + ")";
		if (ImGui::Button(btn_name.c_str()))
		{
			show_log = !show_log;
		}
		ImGui::SameLine();

		ImGui::PopStyleColor();

		//Errors Button

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));

		btn_name = "Error(" + std::to_string(App->imgui->console.GetErrorCount()) + ")";
		if (ImGui::Button(btn_name.c_str()))
		{
			show_error = !show_error;
		}

		ImGui::SameLine();

		ImGui::PopStyleColor();

		if (ImGui::Button("Clear"))
		{
			App->imgui->console.Clear();
		}
		
		if (App->imgui->console.GetBuffer().size() == 0)
		{
			ImGui::End();
			return true;
		}

		ImGui::Spacing(); 
			
		ImGui::BeginChild("Child");

		if (ImGui::IsMouseHoveringWindow())
		{
			App->camera->GetEditorCamera()->LockCamera();
		}
		else
			App->camera->GetEditorCamera()->UnlockCamera();

		for (auto it = App->imgui->console.console_buffer.rbegin(); it != App->imgui->console.console_buffer.rend(); it++)
		{
			switch (it->first)
			{
			case Console_Message_Type::CONSOLE_MLOG:
				if (show_log)
					ImGui::TextColored(log_color, it->second.c_str());
				break;

			case Console_Message_Type::CONSOLE_MDEBUG:
				if (show_debug)
					ImGui::TextColored(debug_color, it->second.c_str());
				break;

			case Console_Message_Type::CONSOLE_MERROR:
				if (show_error)
					ImGui::TextColored(error_color, it->second.c_str());
				break;
			}
		}

		ImGui::EndChild();
	}
	ImGui::End();



	return false;
}
