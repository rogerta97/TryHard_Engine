#pragma once

#include "UI_Element.h"

class ComponentImage; 
class Mesh; 

class UI_Image : public UI_Element
{
public:
	UI_Image(ComponentImage* cmp_container_);
	~UI_Image();

	void Start();
	void Update();
	void CleanUp();
	void Draw(); 

	void CreateDrawSpace(); 
	void DrawImage();

	ComponentImage* cmp_container; 

private: 

	Mesh* draw_space_mesh;		//Plane where the texture (if it has) is going to be displayed
};

