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

private: 

	AABB bounding_box; 
	Color color; 

};

