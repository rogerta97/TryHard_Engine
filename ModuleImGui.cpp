#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "imgui_dock.h"
#include "TextureMSAA.h"
#include "ModuleRenderer3D.h"
#include "Application.h"

#include "UI_Panel.h"
#include "UI_ConfigurationPanel.h"
#include "UI_ScenePanel.h"
#include "UI_ConsolePanel.h"
#include "UI_RandomNumberPanel.h"
#include "UI_InspectorPanel.h"
#include "UI_PerformancePanel.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"




ModuleImGui::ModuleImGui(bool start_enabled)
{
	name = "ImGui";
}

ModuleImGui::~ModuleImGui()
{}

bool ModuleImGui::Start()
{
	show_demo_window = false;
	show_style_editor = false;

	//Initialize Panels 

	config_panel = (UI_ConfigurationPanel*)AddPanel(CONFIGURATION_PANEL);
	scene_panel = (UI_ScenePanel*)AddPanel(SCENE_PANEL);
	console_panel = (UI_ConsolePanel*)AddPanel(CONSOLE_PANEL);
	random_panel = (UI_RandomNumberPanel*)AddPanel(RANDOM_PANEL);
	inspector_panel = (UI_InspectorPanel*)AddPanel(INSPECTOR_PANEL);
	performance_panel = (UI_PerformancePanel*)AddPanel(PERFORMANCE_PANEL);

	std::list<UI_Panel*>::iterator panel = panels_list.begin();

	while (panel != panels_list.end())
	{
		(*panel)->Start();
		panel++;
	}

	ImGui_ImplSdlGL2_Init(App->window->window);
	SetDefaultStyle();

	start_time = performance_timer.Read();
	return true;
}

void ModuleImGui::SetDefaultStyle()
{
	ImGui::StyleColorsDark();
}

void ModuleImGui::SendInput(SDL_Event * e) const
{
		ImGui_ImplSdlGL2_ProcessEvent(e);
}


update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	x_motion = -App->input->GetMouseXMotion();
	y_motion = -App->input->GetMouseYMotion();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	
	if (DrawTopBar() != update_status::UPDATE_CONTINUE)
		return update_status::UPDATE_STOP; 

	App->renderer3D->SetUIPrintSettings();
	DrawDocking();
	App->renderer3D->UpdateRenderSettings();
	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::DrawTopBar()
{
	ImGui::BeginMainMenuBar();

	ImVec2 size = ImGui::GetContentRegionAvail();
	
	size.y+=10;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::IsItemHovered()) {
			LOG("HHE");
		}
		if (ImGui::MenuItem("Exit", "Shift + Esc"))
		{
			return UPDATE_STOP;
		}

		if (ImGui::MenuItem("Save"))
		{
			App->SaveConfigAfterUpdate();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Tools"))
	{
		if (ImGui::MenuItem("Configuration"))
		{
			config_panel->show = !config_panel->show;
		}

		if (ImGui::MenuItem("Style"))
		{
			show_style_editor = !show_style_editor;
		}

		if (ImGui::MenuItem("Console"))
		{
			console_panel->show = !console_panel->show;
		}

		if (ImGui::MenuItem("Random Generator"))
		{
			random_panel->show = !random_panel->show;
		}
		if (ImGui::MenuItem("Performance"))
		{
			performance_panel->show = !performance_panel->show;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Empty"))
		{
			
		}

		if (ImGui::MenuItem("Plane"))
		{			
			GameObject* new_go = App->scene_intro->CreateGameObject();
			ComponentMesh* cmp = (ComponentMesh*)new_go->CreateComponent(CMP_RENDERER);
			cmp->SetMesh(App->resources->mesh_importer->GetMeshByType(MESH_PLANE));
			new_go->AddComponent(cmp);

			inspector_panel->SetGameObject(new_go); 
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("ImGui Demo"))
		{
			show_demo_window = !show_demo_window;
		}

		if (ImGui::MenuItem("GitHub Repo"))
		{
			App->OpenWebBrowser("https://github.com/rogerta97/TryHard_Engine");
		}
		
		if (ImGui::MenuItem("Report a Bug!"))
		{
			App->OpenWebBrowser("https://github.com/rogerta97/TryHard_Engine/issues/new"); 
		}

		if (ImGui::BeginMenu("3rd Parties"))
		{
			if (ImGui::MenuItem("SDL"))
			{
				App->OpenWebBrowser("https://www.libsdl.org/download-2.0.php");
			}

			if (ImGui::MenuItem("ImGui"))
			{
				App->OpenWebBrowser("https://github.com/ocornut/imgui");
			}

			if (ImGui::MenuItem("MathGeoLib"))
			{
				App->OpenWebBrowser("http://clb.demon.fi/MathGeoLib/");
			}

			if (ImGui::MenuItem("DeviceID"))
			{
				App->OpenWebBrowser("https://github.com/rogerta97/TryHard_Engine/issues/new");
			}

			if (ImGui::MenuItem("PCG"))
			{
				App->OpenWebBrowser("http://www.pcg-random.org/");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	int mouse_x = App->input->GetMouseX();
	int mouse_y = App->input->GetMouseY();

	bool clicking = false;
	bool dragging = false;

	if (mouse_x != 0 && mouse_y != 0) {

		if (mouse_x < size.x)
		{
			if (mouse_y < size.y) 
			{
				dragging = true;
			}
		}

	}

	if (x_motion != 0)
		clicking = true;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int x_win_pos, y_win_pos;
		App->window->GetPosition(&x_win_pos, &y_win_pos);

		if (x_motion != 0)
			clicking = true;

		if (clicking)
			LOG("PENE");
	}



	return UPDATE_CONTINUE;

}

update_status ModuleImGui::DrawDocking()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	if (ImGui::Begin("MainDock", 0, window_flags))
	{
		float offset = 18.0f;
		ImGui::SetWindowPos(ImVec2(-5, offset));
		ImGui::SetWindowSize(ImVec2(App->window->GetWidth() + 8, App->window->GetHeight() + offset));

		//Update Panels 
		/*std::list<UI_Panel*>::iterator panel = panels_list.begin();
		while (panel != panels_list.end())
		{
		ImGui::BeginDockspace();
		(*panel)->Update();
		ImGui::EndDockspace();
		ImGui::SetNextDock("Dock Demo", ImGuiDockSlot::ImGuiDockSlot_Right);

		panel++;
		}*/

		if (show_style_editor)
		{
			ImGui::BeginDockspace();
			if (ImGui::BeginDock("Style Editor", &show_style_editor))
			{
				ImGui::ShowStyleEditor();
			}
			ImGui::EndDock();
			ImGui::EndDockspace();
		}

		if (show_demo_window)
		{
			ImGui::BeginDockspace();
			if (ImGui::BeginDock("Demo Editor", &show_demo_window))
			{
				ImGui::ShowTestWindow();
			}
			ImGui::EndDock();
			ImGui::EndDockspace();

		}

		//ImGui::BeginDockspace();
		//hjsagsdj
		//ImGui::EndDockspace();

		ImGui::BeginDockspace();

		console_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		random_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Top);	
		scene_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Right);
		inspector_panel->Update();
		
		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		config_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		performance_panel->Update();

		ImGui::EndDockspace();

		//ImGui::SetNextDock("Configuration##Dock Demo", ImGuiDockSlot::ImGuiDockSlot_Right); To add it as a tab in the same dock

	}
	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}



bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();

	return true;
}

UI_Panel * ModuleImGui::AddPanel(Panel_Types type)
{
	UI_Panel* panel = nullptr;
	switch (type)
	{
	case CONFIGURATION_PANEL:
		panel = new UI_ConfigurationPanel();
		break;
	case SCENE_PANEL:
		panel = new UI_ScenePanel();
		break;
	case CONSOLE_PANEL:
		panel = new UI_ConsolePanel();
		break;
	case RANDOM_PANEL:
		panel = new UI_RandomNumberPanel();
		break;
	case INSPECTOR_PANEL:
		panel = new UI_InspectorPanel();
		break;
	case PERFORMANCE_PANEL:
		panel = new UI_PerformancePanel();
		break;
	default:
		break;
	}

	panels_list.push_back(panel);
	return panel;
}

void ModuleImGui::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}




