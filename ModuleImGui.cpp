#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
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

	show_demo_window = false; 
	show_style_editor = false; 

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

	if (ImGui::BeginMenu("Configuration"))
	{
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
