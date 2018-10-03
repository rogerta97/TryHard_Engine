#pragma once
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class Mesh; 

class ComponentBoundingBox : public Component
{
public:
	ComponentBoundingBox();
	~ComponentBoundingBox();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	void DrawBoundingBox(); 

	bool CreateEnclosedMeshAABB();

private: 

	AABB bounding_box; 

};

