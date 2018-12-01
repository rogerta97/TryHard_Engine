#pragma once
#include "Component.h"

class UI_Image; 

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* go);
	~ComponentImage();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(); 

private:
	UI_Image * image; 
};

