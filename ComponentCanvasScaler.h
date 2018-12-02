#pragma once
#include "Component.h"

#include "Transform.h"

class ComponentCanvasScaler : public Component
{
public:
	ComponentCanvasScaler();
	~ComponentCanvasScaler();

	bool Start();
	bool Update();
	bool CleanUp();

	Transform modified_transform; 
};

