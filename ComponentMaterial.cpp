#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial()
{
}


ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Update()
{
	return false;
}

void ComponentMaterial::SetDiffuseTexture(Texture* tex)
{
	diffuse = tex; 
}

Texture * ComponentMaterial::GetDiffuseTexture() const
{
	return diffuse;
}
