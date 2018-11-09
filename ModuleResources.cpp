#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "Resource.h"

#include "Functions.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Material.h"


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

Resource* ModuleResources::CreateNewResource(resource_type type, UID force_id)
{
	Resource* to_ret = nullptr; 

	switch (type)
	{
	case RES_MESH:
		to_ret = new Mesh();
		break;

	//case RES_TEXTURE:
	//	to_ret = new Texture();		
	//	break; 

	case RES_MATERIAL:
		to_ret = new Material();
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

	return false;
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

		ImGui::Text("Total Resources in Assets: %d", resources.size());

		SEPARATE_WITH_SPACE

		if (ImGui::TreeNode("Meshes"))
		{
			SEPARATE_WITH_SPACE

			ImGui::Text("Mesh Resources in Assets: %d", GetResourcesLoadedAmmount(RES_MESH));

			SEPARATE_WITH_SPACE

			int i = 0; 
			for (auto it = resources.begin(); it != resources.end(); it++)
			{
				if ((*it).second->GetType() == resource_type::RES_MESH)
				{
					Mesh* mesh = (Mesh*)Get(RES_MESH, (*it).second->name.c_str()); 

					ImGui::Text("%d. ", i++); ImGui::SameLine();

					if (ImGui::TreeNode((*it).second->name.c_str()))
					{
						ImGui::Text("Reference Counting: %d", mesh->reference_counting); 

						ImGui::TreePop();
					}
				}
			
			}
			ImGui::TreePop();
		}

		SEPARATE_WITH_SPACE

		if (ImGui::TreeNode("Materials"))
		{
			SEPARATE_WITH_SPACE

			ImGui::Text("Material Resources in Assets: %d", GetResourcesLoadedAmmount(RES_MATERIAL));

			SEPARATE_WITH_SPACE

			int i = 0;
			for (auto it = resources.begin(); it != resources.end(); it++)
			{
				if ((*it).second->GetType() == resource_type::RES_MATERIAL)
				{
					Material* mat = (Material*)Get(RES_MATERIAL, (*it).second->name.c_str());
					ImGui::Text("%d. ", i++); ImGui::SameLine();

					if (ImGui::TreeNode((*it).second->name.c_str()))
					{
						ImGui::Text("Reference Counting: %d", mat->reference_counting);

						ImGui::TreePop();
					}
				}

			}
			ImGui::TreePop();
		}
	}
}


