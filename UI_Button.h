#pragma once
#include "UI_Element.h"

class UI_Image; 
class ComponentButton; 

class UI_Button : public UI_Element
{
public:
	UI_Button(ComponentButton* container);
	~UI_Button();

	void Start();
	void Update();
	void CleanUp();
	void Draw();

	void AddaptToRectSize();
	
	ComponentButton* component_container; 

private: 

	Mesh* clickable_area; 
};

