#pragma once
#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	void SetDiffuseTexture(Texture* tex);
	Texture* GetDiffuseTexture() const; 

private:

	Texture * diffuse; 
	
};

