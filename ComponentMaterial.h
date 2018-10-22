#pragma once
#include "Component.h"
#include "Texture.h"
#include "Material.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

	bool Update();
	bool CleanUp(); 

	Material* GetMaterial() const; 
	void SetMaterial(Material* new_mat); 

private:

	Material * material; 
	
};

