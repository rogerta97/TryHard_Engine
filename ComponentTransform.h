#pragma once

#include "Transform.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	bool Start(); 
	bool Update(); 

private:
	Transform transform; 
};

