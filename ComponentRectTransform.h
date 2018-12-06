#pragma once
#include "Component.h"
#include "Transform.h"
#include "SDL\include\SDL_rect.h"

#include "ComponentTransform.h"

class GameObject;
class ComponentRectTransform; 
class ComponentTransform; 
class ComponentMesh; 
class Mesh;

struct AnchorPoints
{
	float min_x, min_y, max_x, max_y;
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

	float2 GetRelativePos() const;
	void SetRelativePos(float2 new_pos);

	void UpdateRectWithAnchors();

	// ----------------------

	// Set & Get ------------

	ComponentTransform* GetTransform();

	AnchorPoints GetAnchorPoint() const;
	void SetAnchorPoint(float min_x, float min_y, float max_x, float max_y);

	// ----------------------

	float width, height;
	float3 scale_to_show; 

private:

	Mesh * quad_mesh;						//This is the mesh of the canvas, it will be drawn automatically
	ComponentTransform* transform_part; 
	
	AnchorPoints anchor;

	float2 relative_pos;

	ComponentRectTransform* parent;
};



