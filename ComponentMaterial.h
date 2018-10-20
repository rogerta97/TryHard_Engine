#pragma once
#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

	bool Update();
	bool CleanUp(); 

	Texture * diffuse;

private:

	
};

