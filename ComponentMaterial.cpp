#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"

#include "MaterialImporter.h"

ComponentMaterial::ComponentMaterial(GameObject* parent)
{
	SetGameObject(parent);
	component_type = CMP_MATERIAL;
	active = true; 

	material = new Material();
}


ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Update()
{
	return false;
}

bool ComponentMaterial::CleanUp()
{
	if (material != nullptr)
	{
		Texture* curr_tex = material->GetDiffuseTexture();

		if (curr_tex != nullptr)
		{
			//if (!App->scene->IsTextureUsed(curr_tex->GetTextureID(), gameobject))
			//{
			//	curr_tex->Clear();
			//	//App->resources->material_importer->DeleteTextureFromList(curr_tex);
			//	delete (material->GetDiffuseTexture());
			//}

			//material->SetDiffuseTexture(nullptr);

			material = nullptr;
		}
	}

	return true;
}

Material * ComponentMaterial::GetMaterial() const
{
	return material;
}

void ComponentMaterial::SetMaterial(Material * new_mat)
{
	if (new_mat != nullptr)
		material = new_mat;
	else
		CONSOLE_ERROR("You are trying to assign a NULL material"); 
}

void ComponentMaterial::SetColor(Color color)
{
	material->color = color;
}

void ComponentMaterial::Load(JSON_Object * root_obj)
{	
	string diffuse_name = json_object_dotget_string(root_obj, "DiffuseName");

	if (diffuse_name != "NONE" && diffuse_name != "")
	{
		string lib_name = App->file_system->DeleteFileExtension(diffuse_name.c_str());
		material = (Material*)App->resources->Get(RES_MATERIAL, lib_name.c_str());
	}
	else
	{
		material = new Material(); 
		float3 rgb = { (float)json_object_dotget_number(root_obj, "Color.R"), (float)json_object_dotget_number(root_obj, "Color.G"), (float)json_object_dotget_number(root_obj, "Color.B") };
		material->color.Set(rgb.x, rgb.y, rgb.z);
	}
}

void ComponentMaterial::Save(JSON_Object * root_obj, const char* root)
{
	std::string node_name = root;
	std::string item_name = "";

	item_name = node_name + ".Components.ComponentMaterial.DiffuseName";

	if(material->GetDiffuseTexture() == nullptr)
		json_object_dotset_string(root_obj, item_name.c_str(), "NONE");
	else
		json_object_dotset_string(root_obj, item_name.c_str(), material->GetDiffuseTexture()->name.c_str());

	item_name = node_name + ".Components.ComponentMaterial.Color.R";
	json_object_dotset_number(root_obj, item_name.c_str(), material->color.r); 

	item_name = node_name + ".Components.ComponentMaterial.Color.G";
	json_object_dotset_number(root_obj, item_name.c_str(), material->color.g);

	item_name = node_name + ".Components.ComponentMaterial.Color.B";
	json_object_dotset_number(root_obj, item_name.c_str(), material->color.b);
}

