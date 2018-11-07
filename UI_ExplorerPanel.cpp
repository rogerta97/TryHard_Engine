#include "UI_ExplorerPanel.h"
#include "Application.h"

#include "MaterialImporter.h"
#include "MeshImporter.h"

#include "Material.h"

UI_ExplorerPanel::UI_ExplorerPanel()
{
	show_item_options = false; 
	name = "Explorer";

	folder_mat = nullptr;
	image_mat = nullptr;
	mesh_mat = nullptr;
	font_mat = nullptr;
	DDS_mat = nullptr;
	TGA_mat = nullptr;
	scene_mat = nullptr;
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

		if (folder_mat)
		{
			ImGui::Image((ImTextureID)folder_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 15), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
		}
			

		if (ImGui::TreeNodeEx(item_name.c_str()))
		{
			//If it has childs we call them recursively
			vector<string> child_files;
			App->file_system->GetFilesInDirectory(folder.c_str(), child_files, false);

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

				if (mesh_mat == nullptr)
					break; 

				ImGui::Image((ImTextureID)mesh_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break; 

			case file_type::FT_IMAGE:

				if (App->file_system->GetFileExtension(folder.c_str()) == FX_DDS)
				{
					if (DDS_mat == nullptr)
						break;

					ImGui::Image((ImTextureID)DDS_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
					ImGui::SameLine();
				}
				else if (App->file_system->GetFileExtension(folder.c_str()) == FX_TGA)
				{
					if (TGA_mat == nullptr)
						break;

					ImGui::Image((ImTextureID)TGA_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
					ImGui::SameLine();
				}

				else
				{
					if (image_mat == nullptr)
						break;

					ImGui::Image((ImTextureID)image_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
					ImGui::SameLine();
				}
	
				break;

			case file_type::FT_FONT:

				if (font_mat == nullptr)
					break;

				ImGui::Image((ImTextureID)font_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break;

			case file_type::FT_SCENE:

				if (scene_mat == nullptr)
					break;

				ImGui::Image((ImTextureID)scene_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
				ImGui::SameLine();
				break;

			case file_type::FT_PREFAB:

				if (obj_mat == nullptr)
					break;

				ImGui::Image((ImTextureID)obj_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18));
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
				{
					string lib_item = App->file_system->GetLastPathItem(string(folder.c_str() + string(".dds")).c_str()); 
					Material* new_mat = App->resources->material_importer->LoadFromBinary(lib_item.c_str());
				}
					
				else if (App->file_system->GetFileType(folder.c_str()) == FT_SCENE)
					App->scene->LoadScene(item_name.c_str());

				else if (App->file_system->GetFileType(folder.c_str()) == FT_PREFAB)
				{
					string obj_name = App->file_system->GetLastPathItem(folder.c_str()); 

					GameObject* new_go = App->scene->CreateGameObject(obj_name.c_str());
					new_go->LoadPrefab(obj_name.c_str());
				}
					
			
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

	folder_mat = (Material*)App->resources->Get(RES_MATERIAL, "FolderIcon");
	obj_mat = (Material*)App->resources->Get(RES_MATERIAL, "GameObjectIcon");
	image_mat = (Material*)App->resources->Get(RES_MATERIAL, "ImageIcon");
	mesh_mat = (Material*)App->resources->Get(RES_MATERIAL, "MeshIcon");
	font_mat = (Material*)App->resources->Get(RES_MATERIAL, "FontIcon");
	DDS_mat = (Material*)App->resources->Get(RES_MATERIAL, "DDSIcon");
	TGA_mat = (Material*)App->resources->Get(RES_MATERIAL, "TGAIcon");
	scene_mat = (Material*)App->resources->Get(RES_MATERIAL, "SceneIcon");

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
