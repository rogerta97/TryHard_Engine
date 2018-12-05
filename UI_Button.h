#pragma once
#include "UI_Element.h"

class UI_Image; 
class ComponentButton; 
class UI_Plane; 

class UI_Button : public UI_Element
{
public:
	UI_Button(ComponentButton* container);
	~UI_Button();

	void Start();
	void Update();
	void CleanUp();
	void Draw(bool is_editor);
	
	ComponentButton* component_container; 

private: 

	UI_Plane* clickable_area;  //We will just use the plane for the clicking area. 
};

