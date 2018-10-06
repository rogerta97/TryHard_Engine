#pragma once
#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	Texture * diffuse;

private:

	
};

