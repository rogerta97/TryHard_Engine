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
	material->SetDiffuseTexture(nullptr); 	
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
	if (material->GetDiffuseTexture() != nullptr)
	{
		if (!App->scene->IsTextureUsed(material->GetDiffuseTexture()->GetTextureID(), gameobject))
		{
			material->GetDiffuseTexture()->Clear();
			App->resources->material_importer->DeleteTextureFromList(material->GetDiffuseTexture());
		}
	 
		material->SetDiffuseTexture(nullptr);
		delete (material->GetDiffuseTexture());
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

