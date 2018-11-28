#pragma once
#include "Component.h"
#include "Transform.h"

class GameObject;
class ComponentRectTransform; 
class ComponentTransform; 

struct AnchorPoint
{
	float2 min_point, max_point;
	ComponentRectTransform* rect_transform_attached;
};

class ComponentRectTransform : public Component
{
public:

	ComponentRectTransform(GameObject* parent);
	~ComponentRectTransform();

	bool Start();
	bool Update();
	bool CleanUp();

	//Utility functions -----


	// ----------------------

	// Set & Get ------------

	ComponentTransform* GetTransform();

	// ----------------------

private:

	ComponentTransform* transform_part; 
	AnchorPoint anchor; 

	ComponentRectTransform* parent; 
	
};



