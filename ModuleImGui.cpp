#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "imgui_dock.h"
#include "TextureMSAA.h"
#include "ModuleRenderer3D.h"
#include "Application.h"

#include "DevIL Windows SDK\include\IL\il.h"
#include "Assimp\include\version.h"

#include "UI_Panel.h"
#include "UI_ConfigurationPanel.h"
#include "UI_ScenePanel.h"
#include "UI_ConsolePanel.h"
#include "UI_GamePanel.h"
#include "UI_RandomNumberPanel.h"
#include "UI_InspectorPanel.h"
#include "UI_ExplorerPanel.h"
#include "UI_OctreePanel.h"
#include "UI_GamePanel.h"
#include "UI_PerformancePanel.h"
#include "UI_HierarchyPanel.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "TinyFileDialog.h"





ModuleImGui::ModuleImGui(bool start_enabled)
{
	name = "ImGui";
}


ModuleImGui::~ModuleImGui()
{}

bool ModuleImGui::Init(JSON_Object* config)
{
	config_panel = (UI_ConfigurationPanel*)AddPanel(CONFIGURATION_PANEL);
	scene_panel = (UI_ScenePanel*)AddPanel(SCENE_PANEL);
	console_panel = (UI_ConsolePanel*)AddPanel(CONSOLE_PANEL);
	random_panel = (UI_RandomNumberPanel*)AddPanel(RANDOM_PANEL);
	inspector_panel = (UI_InspectorPanel*)AddPanel(INSPECTOR_PANEL);
	performance_panel = (UI_PerformancePanel*)AddPanel(PERFORMANCE_PANEL);
	hierarchy_panel = (UI_HierarchyPanel*)AddPanel(HIERARCHY_PANEL);
	explorer_panel = (UI_ExplorerPanel*)AddPanel(EXPLORER_PANEL); 
	octree_panel = (UI_OctreePanel*)AddPanel(OCTREE_PANEL); 
	game_panel = (UI_GamePanel*)AddPanel(GAME_PANEL); 

	return true;
}

bool ModuleImGui::Start()
{
	std::list<UI_Panel*>::iterator panel = panels_list.begin();

	while (panel != panels_list.end())
	{
		(*panel)->Start();
		panel++;
	}

	ImFont* pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/orkney-regular.otf", 14.0f);
	pFont->DisplayOffset = { 0,1 };
	pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/Quicksand-Regular.otf", 14.0f);

	show_demo_window = false;
	show_style_editor = false;

	last_x = 0;
	last_y = 0;

	ImGui_ImplSdlGL2_Init(App->window->window);
	SetDefaultStyle();

	start_time = performance_timer.Read();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;

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

	is_mouse_in_scene = scene_panel->is_mouse_in;


	App->renderer3D->UseUIRenderSettings();
	DrawDocking();
	ImGui::Render();
	App->renderer3D->UseCurrentRenderSettings();
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::ShowSavePopup()
{


	if (App->imgui->show_save_popup)
		ImGui::OpenPopup("Do you want to save before exit?");

	if (ImGui::BeginPopupModal("Do you want to save before exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("All those beautiful changes will be lost forever.\n");
		ImGui::Separator();

		//static int dummy_i = 0;
		//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

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

		if (ImGui::MenuItem("Save Settings"))
		{
			App->SaveConfigAfterUpdate();
		}

		if (ImGui::MenuItem("Save Scene"))
		{
			char const * lFilterPatterns[1] = { "*.json" };
			const char* path = tinyfd_saveFileDialog("Save Scene...", "UntitledScene", 1, lFilterPatterns, NULL);
			string scene_name = App->file_system->GetLastPathItem(path, true); 

			App->scene->SaveScene(scene_name.c_str());
		}

		if (ImGui::MenuItem("Load Scene"))
		{
			char const * lFilterPatterns[2] = { "*.json" };
			const char* path = tinyfd_openFileDialog("Load Scene...", NULL, 1, lFilterPatterns, NULL, 0);
			string scene_name = App->file_system->GetLastPathItem(path, true);

			App->scene->LoadScene(scene_name.c_str());
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
		if (ImGui::MenuItem("Hierarchy"))
		{
			hierarchy_panel->show = !hierarchy_panel->show;
		}
		if (ImGui::MenuItem("Inspector"))
		{
			inspector_panel->show = !inspector_panel->show;
		}
		if (ImGui::MenuItem("Explorer"))
		{
			explorer_panel->show = !explorer_panel->show;
		}
		if (ImGui::MenuItem("Octree"))
		{
			octree_panel->show = !octree_panel->show;
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Empty"))
		{

			GameObject* new_go = App->scene->CreateGameObject();
			new_go->name = "Empty";
			App->scene->SetSelectedGameObject(new_go);
		}

		if (ImGui::MenuItem("Cube"))
		{
			GameObject* new_go = App->scene->CreateGameObject();
			new_go->SetParent(nullptr); 

			new_go->name = "Cube"; 

			//Add Mesh
			ComponentMesh* cmp = (ComponentMesh*)new_go->AddComponent(CMP_MESH);
			Mesh* new_mesh = App->resources->mesh_importer->GetMeshByType(MESH_CUBE); 

			new_mesh->name = "CubeMesh"; 
			new_mesh->LoadToMemory(); 
			cmp->SetMesh(new_mesh);
			cmp->CreateEnclosedMeshAABB(); 

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
			App->OpenWebBrowser("https://github.com/rogerta97/TryHard_Engine/issues/new");
		

		if (ImGui::BeginMenu("3rd Parties"))
		{
			SDL_version version;
			SDL_GetVersion(&version);

			std::string sdl ("SDL (" + std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch) + ")");

			if (ImGui::MenuItem(sdl.c_str()))
				App->OpenWebBrowser("https://www.libsdl.org/download-2.0.php");
			
			std::string imgui("Dear ImGui (");
			imgui.append(ImGui::GetVersion());
			imgui.append(")");

			if (ImGui::MenuItem(imgui.c_str()))
				App->OpenWebBrowser("https://github.com/ocornut/imgui");
			
			std::string assimp("Assimp (" + std::to_string(aiGetVersionMajor()) + "." + std::to_string(aiGetVersionMinor()) + "." + std::to_string(aiGetVersionRevision()) + ")");

			if (ImGui::MenuItem(assimp.c_str())) 
				App->OpenWebBrowser("http://www.assimp.org/");
			
			if (ImGui::MenuItem("MathGeoLib"))
				App->OpenWebBrowser("http://clb.demon.fi/MathGeoLib/");
			
			if (ImGui::MenuItem("DeviceID"))
				App->OpenWebBrowser("https://github.com/rogerta97/TryHard_Engine/issues/new");
			
			if (ImGui::MenuItem("PCG"))
				App->OpenWebBrowser("http://www.pcg-random.org/");
			
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);

	ImGui::SetCursorPosX(App->window->width - 20);
	if (ImGui::MenuItem("X"))
	{
		show_save_popup = true;
	}

	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);

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
			dragging = true;
		
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
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	bool main_dock_open = true;

	if (ImGui::Begin("MainDock", &main_dock_open, window_flags))
	{
		ShowSavePopup();

		float offset = 12.0f;
		ImGui::SetWindowPos(ImVec2(-5, offset));
		ImGui::SetWindowSize(ImVec2(App->window->GetWidth() + 8, App->window->GetHeight() - offset));

		ImGui::DockSpace(ImGui::GetID("MainDock"));

		if (show_style_editor)
		{
			if (ImGui::Begin("Style Editor", &show_style_editor))
			{
				ImGui::ShowStyleEditor();
			}
			ImGui::End();
		}

		if (show_demo_window)
		{
			if (ImGui::Begin("Demo Editor", &show_demo_window))
			{
				ImGui::ShowTestWindow();
			}
			ImGui::End();
		}

		for (auto panel = panels_list.begin(); panel != panels_list.end(); panel++)
		{

			if ((*panel)->show)
			{
				(*panel)->performance_timer.Start();

				(*panel)->Update();

				if ((*panel)->update_ms_buffer.size() > 50)
					(*panel)->update_ms_buffer.erase((*panel)->update_ms_buffer.begin());

				(*panel)->update_ms_buffer.push_back((*panel)->performance_timer.Read());
			}
		}

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
	case EXPLORER_PANEL:
		panel = new UI_ExplorerPanel();
		break;
	case OCTREE_PANEL:
		panel = new UI_OctreePanel();
		break;
	case GAME_PANEL:
		panel = new UI_GamePanel();
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




