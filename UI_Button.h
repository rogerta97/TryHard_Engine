#pragma once
#include "UI_Element.h"

class UI_Image; 
class ComponentButton; 
class UI_Plane; 

enum Button_Transition
{
	TRANSITION_COLOR,
	TRANSITION_IMG_SWAP,
	TRANSITION_ANY,
};

enum Button_State
{
	BUTTON_HOVER,
	BUTTON_CLICK,
	BUTTON_ENTER,
	BUTTON_OUT,
	BUTTON_IDLE, 
};

class UI_Button : public UI_Element
{
public:
	UI_Button(ComponentButton* container);
	~UI_Button();

	void Start();
	void Update();
	void CleanUp();
	void Draw(bool is_editor);

	UI_Plane* GetArea() const; 
	
	ComponentButton* component_container; 

	Button_Transition GetTransition() const; 
	void SetTransition(Button_Transition new_transition);

private: 

	UI_Plane* clickable_area;		//We will just use the plane for the clicking area. 

	Button_Transition transition_type; 
	Button_State button_state; 
};

