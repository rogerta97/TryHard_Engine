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

	show_demo_window = true; 
	show_style_editor = true;

	//Initialize Panels 
	config_panel = new UI_ConfigurationPanel(); 

	ImGui_ImplSdlGL2_Init(App->window->window);
	ImGui::InitDock(); 
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

	//Update Panels 
	config_panel->Update(); 

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
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;

	if (ImGui::Begin("Dock Demo", 0, window_flags))
	{
		float offset = 18.0f; 
		ImGui::SetWindowPos(ImVec2(-5, offset));
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w + 5, App->window->screen_surface->h - offset));
		// dock layout by hard-coded or .ini file
		ImGui::BeginDockspace();

		if (ImGui::BeginDock("Dock 1")) {
			ImGui::Text("I'm Wubugui!");
		}
		ImGui::EndDock();

		if (ImGui::BeginDock("Dock 2")) {
			ImGui::Text("I'm BentleyBlanks!");
		}
		ImGui::EndDock();

		if (ImGui::BeginDock("Dock 3")) {
			ImGui::Text("I'm LonelyWaiting!");
		}
		ImGui::EndDock();

		ImGui::EndDockspace();
	}
	ImGui::End();

	// multiple dockspace supported
	if (ImGui::Begin("Dock Demo2"))
	{
		ImGui::BeginDockspace();

		if (ImGui::BeginDock("Dock 2")) {
			ImGui::Text("Who's your daddy?");
		}
		ImGui::EndDock();

		ImGui::EndDockspace();
	}
	ImGui::End();

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

	if (show_demo_window) ImGui::ShowDemoWindow(); 
	if (show_style_editor)
	{
		ImGui::Begin("Style Editor", &show_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End(); 
	}

	ImGui::EndMainMenuBar(); 

	return UPDATE_CONTINUE;

}
