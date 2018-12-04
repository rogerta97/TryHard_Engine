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
	
	ComponentButton* component_container; 

private: 

	UI_Image* clickable_area;  //We will just use the plane for the clicking area. 
};

