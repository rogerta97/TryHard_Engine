#include "ComponentMaterial.h"



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

