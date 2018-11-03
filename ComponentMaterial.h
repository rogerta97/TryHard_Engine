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

	//Load & Save
	void Load(JSON_Object* root_obj);
	void Save(JSON_Object* root_obj, const char* root);

	Material* GetMaterial() const; 
	void SetMaterial(Material* new_mat); 
	void SetColor(Color color);

private:

	Material * material; 
	
};

