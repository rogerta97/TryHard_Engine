#pragma once
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Color.h"

class Mesh; 

class ComponentBoundingBox : public Component
{
public:
	ComponentBoundingBox();
	~ComponentBoundingBox();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	bool CreateEnclosedMeshAABB();
	float3 GetBoxCenter(); 

	void SetDraw(bool draw); 

	bool drawbox;

private: 

	AABB bounding_box; 
	Color color; 
};

