#pragma once
#include "Component.h"
#include "UI_Image.h"

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* go);
	~ComponentImage();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor); 

	UI_Image* GetImage() const; 

private:
	UI_Image * image; 
};

