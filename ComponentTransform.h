#pragma once

#include "Transform.h"

class ComponentTransform
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Start(); 
	void Update(); 

private:
	Transform transform; 
};

