#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "imgui_dock.h"
#include "ModuleRenderer3D.h"
#include "UI_ConfigurationPanel.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"

ModuleImGui::ModuleImGui(bool start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Start()
{
	name = "ImGui";

	//Initialize Panels 
	config_panel = new UI_ConfigurationPanel(); 

	ImGui_ImplSdlGL2_Init(App->window->window);
	SetDefaultStyle(); 

	return true;
}

void ModuleImGui::SetDefaultStyle()
{
	ImGui::StyleColorsClassic();
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	
	if (DrawTopBar() != update_status::UPDATE_CONTINUE)
		return update_status::UPDATE_STOP; 

	App->renderer3D->SetUIPrintSettings();
	DrawDocking();
	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{

	ImGui_ImplSdlGL2_Shutdown();

	return true;
}

update_status ModuleImGui::DrawTopBar()
{
	ImGui::BeginMainMenuBar(); 

	if (ImGui::BeginMenu("Files"))
	{
		if (ImGui::MenuItem("Exit", "Shift + Esc"))
		{
			return UPDATE_STOP;
		}
	
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Configuration"))
		{
			config_panel->show = !config_panel->show;
		}

		if (ImGui::MenuItem("Style"))
		{
			show_style_editor = !show_style_editor; 
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Documentation"))
	{
		if (ImGui::MenuItem("ImGui Demo"))
		{
			show_demo_window = !show_demo_window; 
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar(); 

	return UPDATE_CONTINUE;

}

update_status ModuleImGui::DrawDocking()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Dock Demo", 0, window_flags))
	{
		float offset = 18.0f;
		ImGui::SetWindowPos(ImVec2(-5, offset));
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w + 5, App->window->screen_surface->h - offset));
		// dock layout by hard-coded or .ini file
		ImGui::BeginDockspace();

		//Update Panels 	
		config_panel->Update();



		ImGui::EndDockspace();
	}



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

	ImGui::End();

	return update_status::UPDATE_CONTINUE;
}

