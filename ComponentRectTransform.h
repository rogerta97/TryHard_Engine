#ifndef _COMPONENT_RECTTRANSFORM_H_
#define _COMPONENT_RECTTRANSFORM_H_

#include "Component.h"
#include "Transform.h"
#include "SDL\include\SDL_rect.h"
#include "UI_Element.h"

#include "ComponentTransform.h"
#include "MathGeoLib\MathGeoLib.h"

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
	void DrawAnchorPoint(float3 pos, float2 lines_lenght);
	void OnEvent(const Event& new_event);

	//Utility functions -----

	float3 GetGlobalPosition(); 

	void AddaptRectToScreenSize();										// Will be use in case a canvas is added
	void CreateRectQuad();												// Will create the quad of the Rect
	void DrawRectFrame();												// Will create the quad of the Rect
	void DebugDrawRectSize();											// The difference is that this rect is to debug the width and height
	void Resize(float2 new_size);										// Will resize the rectangle at the values sended
	float2 GetSizeFromPercentage(float value, UI_Widgget_Type type);	// Returns the size of the percentage sended. ex. 0.2 would output the 20%
	float2 GetSizeFromCanvasPercentage(const float& percentage);
	float3 GetPointFromPercentage(const float2& percentage); 
	float3 GetRectCenter();
	int GetFontSizeFromRectSize(); 

	void CompensateParentRelativePos();

	float2 GetRelativePos() const;
	void SetRelativePos(float2 new_pos);
	void TranslateRelativePos(float2 new_pos);

	Mesh* GetRectQuad() const; 
	ComponentMesh* GetRectQuadComponent() const;
	float2 GetSize() const; 

	void ResizeWithPercentage();

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	void UpdateRectWithAnchors();

	bool GetClosestIntersectionPoint(LineSegment line, float3 &closest_point, float & distance); //For mousepicking
	bool GetClosestIntersectionPointForGame(LineSegment line, float3 &closest_point, float & distance);

	GameObject* GetFirstCanvasParent();

	void ManageDrag();

	bool isMouseInsideRect(float2 mouse_pos_in_canvas);

	void CalculateRelSizeWithSize();

	// ----------------------

	void FitToParentRect();

	// Set & Get ------------

	ComponentTransform* GetTransform();

	AnchorPoints GetAnchorPoint() const;
	void SetAnchorPoint(float min_x, float min_y, float max_x, float max_y);

	// ----------------------

	float width = 0, height = 0;
	float3 scale_to_show = {0,0,0};
	bool edited = false;
	float2 rel_size = {0,0};

	float percentage_size = 0.0f;

	bool draggable = false;
private:

	ComponentMesh * quad_mesh = nullptr;						//This is the mesh of the canvas, it will be drawn automatically
	ComponentTransform* transform_part = nullptr;
	
	AnchorPoints anchor = AnchorPoints();
	float2 relative_pos = {0,0};

	ComponentRectTransform* parent = nullptr;

	float x_motion = 0, y_motion = 0, last_x = 0, last_y = 0;

};

#endif



