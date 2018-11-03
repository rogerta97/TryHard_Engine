#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"

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
			if (!App->scene->IsTextureUsed(curr_tex->GetTextureID(), gameobject))
			{
				curr_tex->Clear();
				App->resources->material_importer->DeleteTextureFromList(curr_tex);
				delete (material->GetDiffuseTexture());
			}

			material->SetDiffuseTexture(nullptr);
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

	string diffuse_path = App->file_system->GetTexturesPath() +  string("\\") + diffuse_name;
	diffuse_path += ".dds"; 

	material->SetDiffuseTexture(App->resources->material_importer->LoadTexture(diffuse_path.c_str()));
}

void ComponentMaterial::Save(JSON_Object * root_obj, const char* root)
{
	std::string node_name = root;
	std::string item_name = "";

	item_name = node_name + ".Components.ComponentMaterial.DiffuseName";
	if(material->GetDiffuseTexture() == nullptr)
		json_object_dotset_string(root_obj, item_name.c_str(), "NONE");
	else
		json_object_dotset_string(root_obj, item_name.c_str(), material->GetDiffuseTexture()->GetName().c_str());
}

