#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"

ComponentMaterial::ComponentMaterial()
{
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
		diffuse->Clear();
		diffuse = nullptr;
		delete (diffuse);
	}

	return true;
}

