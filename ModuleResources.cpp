#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "Resource.h"

#include "Functions.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Material.h"

#include "Prefab.h"

#include "mmgr\mmgr.h"


ModuleResources::ModuleResources()
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
}

Resource * ModuleResources::Get(UID uid)
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).first == uid)
			return (*it).second;
	}

	return nullptr; 
}

Resource * ModuleResources::Get(resource_type type, const char * resource_name)
{ 
	string res_name(resource_name); 
	int pos = res_name.find_last_of(".");
	if (pos != -1)
	{
		res_name = App->file_system->DeleteFileExtension(res_name.c_str());
	}

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).second->GetType() == type && App->file_system->DeleteFileExtension((*it).second->name.c_str()) == resource_name)
			return (*it).second;
	}

	return nullptr; 
}



int ModuleResources::GetResourcesLoadedAmmount(resource_type type)
{
	int ret_ammount = 0;

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).second->GetType() == type)
			ret_ammount++;
	}

	return ret_ammount;

}

int ModuleResources::GetResourcesUsedAmmount(resource_type type)
{
	int ret_ammount = 0;

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if (((*it).second->GetType() == type || type == RES_NULL) && (*it).second->reference_counting > 0)
			ret_ammount++;
	}

	return ret_ammount;
}

std::list<Resource*> ModuleResources::GetResourcesByType(resource_type type)
{
	if (type == RES_NULL)
		return std::list<Resource*>();

	std::list<Resource*> ret_list; 

	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).second->GetType() == type)
			ret_list.push_back((*it).second);
	}

	return ret_list;
}

void ModuleResources::ManageNewFolderFile(string new_file_path)
{
	//First we should get what type of file has been added 
	file_type type = App->file_system->GetFileType(new_file_path);

	switch (type)
	{
	case FT_3DMODEL:
		//We load the file as normally, the function will save the file as binary (in case it doesn't exist) and will create the resource.
		App->resources->mesh_importer->ManageNewItem(new_file_path.c_str()); 

	case FT_IMAGE:
		//If the image exist in Library\\Materials as .dds we just create the resource, if not we export it and create the resource. 
		App->resources->material_importer->ManageNewTexture(new_file_path); 

		break; 
	}
}

Resource* ModuleResources::CreateNewResource(resource_type type, UID force_id)
{
	Resource* to_ret = nullptr; 

	switch (type)
	{
	case RES_MESH:
		to_ret = new Mesh();
		break;

	case RES_MATERIAL:
		to_ret = new Material();
		break;

	case RES_PREFAB:
		to_ret = new Prefab();
		break;

	case RES_SCENE:
		to_ret = new Scene();
		break;
	}

	if (to_ret)
	{
		to_ret->SetType(type);
		to_ret->SetUID(GenerateUID());
		resources[to_ret->GetUID()] = to_ret;
	}
		
	return resources[to_ret->GetUID()];
}

void ModuleResources::AddTextureToList(Texture* new_texture)
{
	
}

UID ModuleResources::GenerateUID()
{
	UID	ret_id = GetPCGRandomNumberInt(0, 9999999);
	return ret_id;
}

bool ModuleResources::Init(JSON_Object* config)
{
	return true; 
}

bool ModuleResources::Start()
{
	//Initiallize importers
	bool ret = true; 
	LoadImporters();
	return ret;
}

void ModuleResources::LoadImporters()
{
	//Initiallize importers
	material_importer = new MaterialImporter();
	material_importer->Start();

	mesh_importer = new MeshImporter();
	mesh_importer->Start();
}

update_status ModuleResources::Update()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResources::PostUpdate(float dt)
{
	if (WantToDelete())
		DeleteResourcesNow(); 

	return update_status::UPDATE_CONTINUE;
}


bool ModuleResources::CleanUp()
{
//	material_importer->SaveTexturesAsDDS();
// Create a map iterator and point to beginning of map
	auto it = resources.begin();

	// Iterate over the map using Iterator till end.
	while (it != resources.end())
	{
		// Accessing VALUE from element pointed by it.
		Resource* resource = it->second;

		delete resource;

		// Increment the Iterator to point to next entry
		it++;
	}


	material_importer->CleanUp();
	mesh_importer->CleanUp();

	//delete material_importer;
	return true;
}

void ModuleResources::ManageDropedFile()
{
	//If a new file is droped, we need to save the meshes to binary first and create the resources

	file_extension file_dropped_extension = App->file_system->GetFileExtension(App->input->GetFileDroped());

	App->scene->CleanScene(); //Delete if you feel like :)

	Timer test;
	test.Start();

	switch (file_dropped_extension)
	{
		case file_extension::FX_FBX:

			//Create the resource
			GameObject * parent = App->resources->mesh_importer->CreateFBXMesh(App->input->GetFileDroped().c_str());
			App->scene->SetSelectedGameObject(parent);

			//Camera interpolation
			App->camera->GetEditorCamera()->interpolation.interpolate = true;
			App->camera->GetEditorCamera()->interpolation.interpolation_timer.Start();
			App->camera->GetEditorCamera()->FillInterpolationSegmentAndRot();
			break;
	}
		
	CONSOLE_ERROR("Loaded in %d ms", test.Read());

}

void ModuleResources::RecieveEvent(const Event & curr_event)
{
	switch (curr_event.type)
		{
			case FILE_DROPED:
				ManageDropedFile();
				break; 
		}
	
		

		//file_extension file_dropped_extension = App->file_system->GetFileExtension(App->input->GetFileDroped());

		//if (file_dropped_extension == file_extension::FX_FBX)
		//{
		//	App->scene->CleanScene();
		//	Timer test;
		//	test.Start();

		//	GameObject* parent = App->resources->mesh_importer->CreateFBXMesh(App->input->GetFileDroped().c_str());
		//	App->scene->SetSelectedGameObject(parent);

		//	CONSOLE_ERROR("Loaded in %d ms", test.Read());

		//	App->camera->GetEditorCamera()->interpolation.interpolate = true;
		//	App->camera->GetEditorCamera()->interpolation.interpolation_timer.Start();
		//	App->camera->GetEditorCamera()->FillInterpolationSegmentAndRot();

		//}

		//else if (file_dropped_extension == file_extension::FX_PNG || file_dropped_extension == file_extension::FX_DDS || file_dropped_extension == file_extension::FX_JPG)
		//{
		//	/*	GameObject* current_go = nullptr;
		//	current_go = App->scene->GetSelectedGameObject();

		//	if (current_go != nullptr)
		//	{
		//	Texture* text = App->resources->material_importer->LoadTexture(file_droped.c_str());

		//	ComponentMaterial* mat = (ComponentMaterial*)current_go->GetComponent(CMP_MATERIAL);

		//	if (mat == nullptr)
		//	{
		//	CONSOLE_ERROR("Texture can not be dragged with no Material on Destination");
		//	}
		//	else
		//	{
		//	mat->GetMaterial()->SetDiffuseTexture(text);
		//	}
		//	}
		//	else
		//	CONSOLE_ERROR("Could not load texture as there is no Game Object");*/
		//}
	
}

void ModuleResources::AddResourceToDelete(UID to_del_id)
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).second->GetUID() == to_del_id)
			to_del_list.push_back((*it).second->GetUID()); 
	}
}

void ModuleResources::DeleteResourcesNow()
{
	for (auto it = to_del_list.begin(); it != to_del_list.end();it++)
	{
		//Should do the CleanUp	
		Resource* to_del = Get((*it)); 
		DeleteFromList((*it)); 
		delete(to_del);
	}

	to_del_list.clear();
}

bool ModuleResources::WantToDelete()
{
	return !to_del_list.empty(); 
}

void ModuleResources::DeleteFromList(UID to_del_id)
{
	for (auto it = resources.begin(); it != resources.end();)
	{		
		if ((*it).first == to_del_id)
		{
			resources.erase(to_del_id);
			break;
		}			
		else
			it++;
	}
}

void ModuleResources::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Resources"))
	{
		ImGui::Spacing(); 

		ImGui::Text("Total Resources in Assets: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", resources.size()); 

		ImGui::Text("Total Resources Used: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesUsedAmmount());

		SEPARATE_WITH_SPACE

		if (ImGui::TreeNode("Meshes"))
		{

			SEPARATE_WITH_SPACE

			ImGui::Text("Mesh Resources in Assets: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesLoadedAmmount(RES_MESH));

			ImGui::Text("Mesh Resources Used: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesUsedAmmount(RES_MESH));

			SEPARATE_WITH_SPACE

			int i = 0; 
			for (auto it = resources.begin(); it != resources.end(); it++)
			{
				if ((*it).second->GetType() == resource_type::RES_MESH)
				{
					Mesh* mesh = (Mesh*)Get(RES_MESH, (*it).second->name.c_str()); 

					ImGui::Text("%d. ", i++); ImGui::SameLine();

					
					ImGui::Selectable((*it).second->name.c_str());
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();

						ImGui::Text("  Reference Counting: %d", mesh->reference_counting);
						ImGui::Text("  Loaded To Memory: "); ImGui::SameLine();

						if(mesh->IsLoadedToMemory())
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0, 1.0f), "YES");
						else
							ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0, 1.0f), "NO");

						ImGui::EndTooltip();
					}
				}
			
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Materials"))
		{
			SEPARATE_WITH_SPACE

			ImGui::Text("Material Resources in Assets: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesLoadedAmmount(RES_MATERIAL));

			ImGui::Text("Material Resources Used: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesUsedAmmount(RES_MATERIAL));

			SEPARATE_WITH_SPACE

			int i = 0;
			for (auto it = resources.begin(); it != resources.end(); it++)
			{
				if ((*it).second->GetType() == resource_type::RES_MATERIAL)
				{
					Material* mat = (Material*)Get(RES_MATERIAL, (*it).second->name.c_str());
					ImGui::Text("%d. ", i++); ImGui::SameLine();

					ImGui::Selectable((*it).second->name.c_str());
					if (ImGui::IsItemHovered())
					{

						ImGui::BeginTooltip(); 

						ImGui::Text("Reference Counting: %d", mat->reference_counting);
						ImGui::Text("Loaded To Memory: "); ImGui::SameLine();

						if (mat->IsLoadedToMemory())
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0, 1.0f), "YES");
						else
							ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0, 1.0f), "NO");

						ImGui::EndTooltip();
					}
				}

			}
			ImGui::TreePop();
		}

		/*if (ImGui::TreeNode("Prefabs"))
		{
			ImGui::Text("Prefab Resources in Assets: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesLoadedAmmount(RES_PREFAB));

			ImGui::Text("Prefab Resources Used: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0, 1.0f), "%d", GetResourcesUsedAmmount(RES_PREFAB));

			int i = 0;
			for (auto it = resources.begin(); it != resources.end(); it++)
			{
				if ((*it).second->GetType() == resource_type::RES_PREFAB)
				{
					Prefab* mat = (Prefab*)Get(RES_PREFAB, (*it).second->name.c_str());
					ImGui::Text("%d. ", i++); ImGui::SameLine();

					ImGui::Selectable((*it).second->name.c_str());
					if (ImGui::IsItemHovered())
					{

						ImGui::BeginTooltip();

						ImGui::Text("  Reference Counting: %d", mat->reference_counting);
						ImGui::Text("  Loaded To Memory: "); ImGui::SameLine();

						if (mat->IsLoadedToMemory())
							ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0, 1.0f), "YES");
						else
							ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0, 1.0f), "NO");

						ImGui::EndTooltip();
					}
				}

			}
		ImGui::TreePop();*/
		

	}
}


