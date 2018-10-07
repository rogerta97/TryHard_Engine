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
#include "UI_HierarchyPanel.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

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

	last_x = 0;
	last_y = 0;

	//Initialize Panels 

	config_panel = (UI_ConfigurationPanel*)AddPanel(CONFIGURATION_PANEL);
	scene_panel = (UI_ScenePanel*)AddPanel(SCENE_PANEL);
	console_panel = (UI_ConsolePanel*)AddPanel(CONSOLE_PANEL);
	random_panel = (UI_RandomNumberPanel*)AddPanel(RANDOM_PANEL);
	inspector_panel = (UI_InspectorPanel*)AddPanel(INSPECTOR_PANEL);
	performance_panel = (UI_PerformancePanel*)AddPanel(PERFORMANCE_PANEL);
	hierarchy_panel = (UI_HierarchyPanel*)AddPanel(HIERARCHY_PANEL);

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

update_status ModuleImGui::ShowSavePopup()
{


	if (App->imgui->show_save_popup)
		ImGui::OpenPopup("Are you sure you want to exit?");
	if (ImGui::BeginPopupModal("Are you sure you want to exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("All those beautiful changes will be lost forever.\n");
		ImGui::Separator();

		//static int dummy_i = 0;
		//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		ImGui::PopStyleVar();

		if (ImGui::Button("Yes please", ImVec2(120, 0)))
		{
			show_save_popup = false;
			ImGui::CloseCurrentPopup();
			App->SaveConfigAfterUpdate();
			close_on_next_update = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0))) 
		{
			show_save_popup = false;
			ImGui::CloseCurrentPopup();
			close_on_next_update = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{ 
			show_save_popup = false;
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}

	return update_status();
}

update_status ModuleImGui::DrawTopBar()
{
	if (close_on_next_update)
		return UPDATE_STOP;

	ImGui::BeginMainMenuBar();

	ImVec2 size = ImGui::GetContentRegionAvail();

	size.y += 10;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::IsItemHovered()) {
			LOG("HHE");
		}
		if (ImGui::MenuItem("Exit", "Shift + Esc"))
		{
			show_save_popup = true;
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
			GameObject* new_go = App->scene->CreateGameObject();
			new_go->name = "Plane";
			
			//Add Mesh
			ComponentMesh* cmp = (ComponentMesh*)new_go->CreateComponent(CMP_RENDERER);
			cmp->SetMesh(App->resources->mesh_importer->GetMeshByType(MESH_PLANE));
			new_go->AddComponent(cmp);

			//Add sample texture
			ComponentMaterial* mat = (ComponentMaterial*)new_go->CreateComponent(CMP_MATERIAL);
			std::string path = App->file_system->GetTexturesPath().c_str() + std::string("Baker_house.png"); 
			Texture* generated_texture = App->resources->texture_importer->LoadTexture(path.c_str());
			mat->diffuse = generated_texture;

			new_go->AddComponent(mat);

			App->scene->SetSelectedGameObject(new_go);
		}
		if (ImGui::MenuItem("Cube"))
		{
			GameObject* new_go = App->scene->CreateGameObject();
			new_go->name = "Cube"; 

			//Add Mesh
			ComponentMesh* cmp = (ComponentMesh*)new_go->CreateComponent(CMP_RENDERER);
			cmp->SetMesh(App->resources->mesh_importer->GetMeshByType(MESH_CUBE));
			cmp->CreateEnclosedMeshAABB(); 
			new_go->AddComponent(cmp);

			ComponentMaterial* mat = (ComponentMaterial*)new_go->CreateComponent(CMP_MATERIAL);
			//mat->SetDiffuseTexture(nullptr); 
			new_go->AddComponent(mat);

			App->scene->SetSelectedGameObject(new_go);
		}

		if (ImGui::MenuItem("Sphere"))
		{
			GameObject* new_go = App->scene->CreateGameObject();
			new_go->name = "Sphere";

			//Add Mesh
			ComponentMesh* cmp = (ComponentMesh*)new_go->CreateComponent(CMP_RENDERER);
			cmp->SetMesh(App->resources->mesh_importer->GetMeshByType(MESH_SPHERE));
			new_go->AddComponent(cmp);

			App->scene->SetSelectedGameObject(new_go);
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

	ImGui::SetCursorPosX(App->window->width - 20);
	if (ImGui::MenuItem("X"))
	{
		show_save_popup = true;
	}

	ImGui::EndMainMenuBar();


	// Make it Draggable
	int x_win_pos, y_win_pos;
	App->window->GetPosition(&x_win_pos, &y_win_pos);

	int global_mouse_x, global_mouse_y;
	SDL_GetGlobalMouseState(&global_mouse_x, &global_mouse_y);



	int mouse_x = global_mouse_x - x_win_pos;
	int mouse_y = global_mouse_y - y_win_pos;

	if (mouse_x < size.x && mouse_x > 0)
	{
		if (mouse_y < size.y)
		{
			dragging = true;
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && dragging)
	{
		int dx = App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		if (dx == 0 && dy == 0)
			return UPDATE_CONTINUE;

		App->window->SetPosition(x_win_pos + (global_mouse_x - last_x), y_win_pos + (global_mouse_y - last_y));
	}
	else
		dragging = false;

	last_x = global_mouse_x;
	last_y = global_mouse_y;


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

		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		//random_panel->Update();

		hierarchy_panel->Update();


		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Right);
		inspector_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		performance_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		config_panel->Update();



		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Left);
		scene_panel->Update();

		ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Bottom);
		console_panel->Update();




		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Top);
		//scene_panel->Update();

		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Right);
		//inspector_panel->Update();

		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		//config_panel->Update();

		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		//performance_panel->Update();

		//ImGui::SetNextDock("MainDock", ImGuiDockSlot::ImGuiDockSlot_Tab);
		//hierarchy_panel->Update();

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
	case HIERARCHY_PANEL:
		panel = new UI_HierarchyPanel();
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




