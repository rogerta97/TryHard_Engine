#include "UI_ExplorerPanel.h"
#include "Application.h"

#include "MaterialImporter.h"
#include "MeshImporter.h"

#include "Material.h"
#include "Prefab.h"

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

		bool must_hide = false;

		switch (App->file_system->GetFileType(folder.c_str()))
		{
		case file_type::FT_3DMODEL:

			if (mesh_mat == nullptr)
				break;

			ImGui::Image((ImTextureID)mesh_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			break;

		case file_type::FT_IMAGE:

			/*if (App->file_system->GetFileExtension(folder.c_str()) == FX_DDS)
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
			*/
			//else
			//{
			if (image_mat == nullptr)
				break;

			ImGui::Image((ImTextureID)image_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			//}

			break;

		case file_type::FT_FONT:

			if (font_mat == nullptr)
				break;

			ImGui::Image((ImTextureID)font_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			break;

		case file_type::FT_SCENE:

			if (scene_mat == nullptr)
				break;

			ImGui::Image((ImTextureID)scene_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			break;

		case file_type::FT_PREFAB:

			if (obj_mat == nullptr)
				break;

			ImGui::Image((ImTextureID)obj_mat->GetDiffuseTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			break;

		default:
			if (item_name == "MetaMeshes")
				must_hide = true;
			else {
				ImGui::Image((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
			}

		}

		if (!must_hide) {
			ImGui::MenuItem(item_name.c_str());

			if (ImGui::IsItemClicked(1))
			{
				show_item_options = true;
				ImGui::OpenPopup("ExplorerOptions");
				item_selected = item_name;
			}
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
					//If the prefab meta exist we load the binary

					string directory = App->file_system->DeleteLastPathItem(folder.c_str());
					string meta_name = App->file_system->GetLastPathItem(folder, true) + ".meta";

					if (App->file_system->IsFileInDirectory(directory, meta_name.c_str()))
					{
						Prefab* curr_prf = (Prefab*)App->resources->Get(RES_PREFAB, App->file_system->GetLastPathItem(folder).c_str());
						curr_prf->LoadFromBinary();
						curr_prf->GetRootGameObject()->ModifyIDSet();

						App->scene->SetSelectedGameObject(curr_prf->GetRootGameObject());

					}
					else
					{
						GameObject* parent = App->resources->mesh_importer->CreateFBXMesh(folder.c_str());
						App->scene->SetSelectedGameObject(parent);
					}

					App->camera->GetEditorCamera()->center_next_frame = true;

				}

				else if (App->file_system->GetFileExtension(folder.c_str()) == FX_PNG || App->file_system->GetFileExtension(folder.c_str()) == FX_DDS || App->file_system->GetFileExtension(folder.c_str()) == FX_JPG)
				{
					string lib_item = App->file_system->GetLastPathItem(string(folder.c_str() + string(".dds")).c_str());
					Material* new_mat = (Material*)App->resources->CreateNewResource(RES_MATERIAL); //LOOK
					App->resources->material_importer->LoadFromBinary(lib_item.c_str(), new_mat);
				}

				else if (App->file_system->GetFileType(folder.c_str()) == FT_SCENE)
				{
					App->scene->LoadScene(item_name.c_str());
					CONSOLE_LOG("SCENE LOADED SUCCESFULLY");
				}


				else if (App->file_system->GetFileType(folder.c_str()) == FT_PREFAB)
				{
					App->scene->CleanScene();
					string obj_name = App->file_system->GetLastPathItem(folder.c_str());

					GameObject* new_go = App->scene->LoadPrefab(obj_name.c_str());
					new_go->ModifyIDSet();

				}


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
	folder_mat->LoadToMemory();
	folder_mat->reference_counting++;

	obj_mat = (Material*)App->resources->Get(RES_MATERIAL, "GameObjectIcon");
	obj_mat->LoadToMemory();
	obj_mat->reference_counting++;

	image_mat = (Material*)App->resources->Get(RES_MATERIAL, "ImageIcon");
	image_mat->LoadToMemory();
	image_mat->reference_counting++;

	mesh_mat = (Material*)App->resources->Get(RES_MATERIAL, "MeshIcon");
	mesh_mat->LoadToMemory();
	mesh_mat->reference_counting++;

	font_mat = (Material*)App->resources->Get(RES_MATERIAL, "FontIcon");
	font_mat->LoadToMemory();
	font_mat->reference_counting++;

	DDS_mat = (Material*)App->resources->Get(RES_MATERIAL, "DDSIcon");
	DDS_mat->LoadToMemory();
	DDS_mat->reference_counting++;

	TGA_mat = (Material*)App->resources->Get(RES_MATERIAL, "TGAIcon");
	TGA_mat->LoadToMemory();
	TGA_mat->reference_counting++;

	scene_mat = (Material*)App->resources->Get(RES_MATERIAL, "SceneIcon");
	scene_mat->LoadToMemory();
	scene_mat->reference_counting++;

	return true;
}

bool UI_ExplorerPanel::Update()
{
	if (ImGui::Begin("Explorer", &show, NULL))
	{
		DrawExplorerRecursive(root_folder);
	}
	ImGui::End();
	return true;
}

bool UI_ExplorerPanel::CleanUp()
{
	return true;
}
