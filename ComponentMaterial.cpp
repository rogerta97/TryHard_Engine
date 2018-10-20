#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"

ComponentMaterial::ComponentMaterial(GameObject* parent)
{
	SetGameObject(parent);
	component_type = CMP_MATERIAL;
	active = true; 
	diffuse = nullptr; 
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
	if (diffuse != nullptr)
	{
		if (!App->scene->IsTextureUsed(diffuse->GetTextureID(), gameobject))
		{
			diffuse->Clear();
			App->resources->texture_importer->DeleteTextureFromList(diffuse);
		}
	 
		diffuse = nullptr;
		delete (diffuse);
	}

	return true;
}

