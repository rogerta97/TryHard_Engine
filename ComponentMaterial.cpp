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

