#include "UI_ExplorerPanel.h"
#include "Application.h"

#include "MaterialImporter.h"
#include "MeshImporter.h"

UI_ExplorerPanel::UI_ExplorerPanel()
{
	show_item_options = false; 
	name = "Explorer";
}


UI_ExplorerPanel::~UI_ExplorerPanel()
{
}

void UI_ExplorerPanel::DrawExplorerRecursive(std::string folder)
{
	string item_name; 

	if (App->file_system->IsFolder(folder.c_str()))
	{
		item_name = App->file_system->GetLastPathItem(folder.c_str(), true);

		ImGui::Image((ImTextureID)folder_texture->GetTextureID(), ImVec2(18, 15)); ImGui::SameLine();

		if (ImGui::TreeNodeEx(item_name.c_str()))
		{
			//If it has childs we call them recursively
			vector<string> child_files = App->file_system->GetFilesInDirectory(folder.c_str());

			for (auto it = child_files.begin(); it != child_files.end(); it++)
			{
				if ((*it) != "." && (*it) != "..")
				{
					string next_file = folder + '\\' + (*it);
					DrawExplorerRecursive(next_file);
				}
					
			}				
			ImGui::TreePop();
		}
	}
	else
	{
		item_name = App->file_system->GetLastPathItem(folder.c_str(), true);
		file_extension ext = App->file_system->GetFileExtension(folder.c_str());

		switch (App->file_system->GetFileType(folder.c_str()))
		{
			case file_type::FT_3DMODEL:
				ImGui::Image((ImTextureID)mesh_texture->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break; 

			case file_type::FT_IMAGE:
			/*	if(App->file_system->GetFileExtension(folder.c_str()) == FX_DDS)
					ImGui::Image((ImTextureID)DDS_texture->GetTextureID(), ImVec2(18, 18));
				else if (App->file_system->GetFileExtension(folder.c_str()) == FX_TGA)
				{
					ImGui::Image((ImTextureID)DDS_texture->GetTextureID(), ImVec2(18, 18));
				}
				else*/
				ImGui::Image((ImTextureID)image_texture->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break;

			case file_type::FT_FONT:
				ImGui::Image((ImTextureID)font_texture->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break;

			case file_type::FT_SCENE:
				ImGui::Image((ImTextureID)scene_texture->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break;
				
			default: 
				ImGui::Image((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
			
		}
			
		ImGui::MenuItem(item_name.c_str());

		if (ImGui::IsItemClicked(1))
		{
			show_item_options = true;
			ImGui::OpenPopup("ExplorerOptions");
			item_selected = item_name; 
		}		
	}

	if (show_item_options && item_name == item_selected)
	{	
		if (ImGui::BeginPopup("ExplorerOptions"))
		{
			if (ImGui::MenuItem("Load Item"))
			{
				if (App->file_system->GetFileExtension(folder.c_str()) == FX_FBX)
				{
					GameObject* parent = App->resources->mesh_importer->CreateFBXMesh(folder.c_str());
					App->scene->SetSelectedGameObject(parent);

					App->camera->GetEditorCamera()->center_next_frame = true; 
				}
					
				else if (App->file_system->GetFileExtension(folder.c_str()) == FX_PNG || App->file_system->GetFileExtension(folder.c_str()) == FX_DDS || App->file_system->GetFileExtension(folder.c_str()) == FX_JPG)
					App->resources->material_importer->LoadTexture(folder.c_str());

				else if (App->file_system->GetFileType(folder.c_str()) == FT_SCENE)
					App->scene->LoadScene(item_name.c_str());
			
			}

			if (ImGui::MenuItem("Delete Resource")) 
			{
				
			}

			if (ImGui::MenuItem("Open In Explorer"))
			{
				const char* parent_folder = App->file_system->DeleteLastPathItem(folder.c_str()).c_str(); 
				ShellExecute(NULL, "open", parent_folder, NULL, NULL, SW_SHOWMINIMIZED);
			}
			
			ImGui::EndPopup();
		}
	}

	return; 
}

bool UI_ExplorerPanel::Start()
{
	show = true; 
	root_folder = App->file_system->GetAssetsPath(); 
	item_selected = ""; 

	folder_texture = App->resources->material_importer->GetTexture("FolderIcon");
	image_texture = App->resources->material_importer->GetTexture("ImageIcon");
	mesh_texture = App->resources->material_importer->GetTexture("MeshIcon");
	font_texture = App->resources->material_importer->GetTexture("FontIcon");
	DDS_texture = App->resources->material_importer->GetTexture("DDSIcon");
	TGA_texture = App->resources->material_importer->GetTexture("TGAIcon");
	scene_texture = App->resources->material_importer->GetTexture("SceneIcon");

	return true;
}

bool UI_ExplorerPanel::Update()
{
	if (ImGui::Begin("Explorer", &show, NULL))
	{
		DrawExplorerRecursive(root_folder);

		ImGui::End();
	}
	return true;
}

bool UI_ExplorerPanel::CleanUp()
{
	return true;
}
