#pragma once
#include "Component.h"
#include "Transform.h"
#include "SDL\include\SDL_rect.h"

class GameObject;
class ComponentRectTransform; 
class ComponentTransform; 
class ComponentMesh; 
class Mesh;

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
	void Draw(bool is_editor);

	//Utility functions -----

	void AddaptRectToScreenSize();			// Will be use in case a canvas is added
	void CreateRectQuad();					// Will create the quad of the Rect
	void DrawRectFrame();					// Will create the quad of the Rect
	void Resize(float2 new_size);			// Will resize the rectangle at the values sended

	// ----------------------

	// Set & Get ------------

	ComponentTransform* GetTransform();

	float2 GetAnchorPoint() const;
	void SetAnchorPoint(float2 new_point); 

	// ----------------------

private:

	Mesh * quad_mesh;						//This is the mesh of the canvas, it will be drawn automatically
	ComponentTransform* transform_part; 
	
	float2 anchor_point; 
	SDL_Rect rect; 

	ComponentRectTransform* parent;
};



