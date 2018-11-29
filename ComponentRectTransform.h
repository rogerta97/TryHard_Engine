#pragma once
#include "Component.h"
#include "Transform.h"
#include "SDL\include\SDL_rect.h"

class GameObject;
class ComponentRectTransform; 
class ComponentTransform; 

struct AnchorPoint
{
	float2 anchor;
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

	void AddaptRectToScreenSize();			// Will be use in case a canvas is added

	// ----------------------

	// Set & Get ------------

	ComponentTransform* GetTransform();

	float2 GetAnchorPoint() const;
	void SetAnchorPoint(float2 new_point); 

	// ----------------------

private:

	ComponentTransform* transform_part; 
	ComponentRectTransform* parent;

	float2 anchor_point; 
	SDL_Rect rect; 

		
};



