#ifndef _UI_ELEMENT_H_
#define _UI_ELEMENT_H_

#include "GameObject.h"

class Mesh; 
class UI_Canvas;

enum UI_Widgget_Type
{
	UI_BUTTON,
	UI_CHECKBOX,
	UI_LABEL,
	UI_IMAGE,
	UI_INPUTFIELD,
	UI_NULL,
};

enum UI_ElementState
{
	ELM_PRESSED,
	ELM_UP,
	ELM_IDLE,
	ELM_HOVERED,
	ELM_ANY,
};

class UI_Element
{
public:
	UI_Element();
	UI_Element(UI_Widgget_Type type);
	~UI_Element();

	virtual void Start(); 
	virtual void Update();
	virtual void CleanUp();
	virtual void Draw(); 

	UI_Widgget_Type GetType() const;
	void SetType(const UI_Widgget_Type& new_type);

	float GetPercentage() const;
	void SetPercentage(float new_num);

	bool GetRaycast() const;
	void SetRaycast(bool new_ray);

	GameObject* GetContainerCanvasGO() const;
	UI_Canvas* GetCanvas() const;
	void SetCanvas(UI_Canvas* new_ray);

	void SetState(const UI_ElementState& new_state); 
	UI_ElementState GetState() const;

	void SetSelected(const bool& new_state);
	bool GetSelected() const;

	bool IsRenderElement() const; 
	void SetRenderElement(const bool& re); 

	float GetAlphaPercentage() const;
	void SetAlphaPercentage(const float& re);

private:

	bool selected = false;									// Is element selected
	bool render_element = true;								// Element should be rendered

	float alpha_percentage = 1.0f;							// Percentage of alpha (used for interpolation)

	UI_ElementState state = UI_ElementState::ELM_IDLE;		// State of the element 
	UI_Widgget_Type wid_type;								// Type of UI element 
	UI_Canvas* canvas;										// Canvas where the element lays
	bool raycast;											// Make the canvas clickable
	float canvas_percentage;								// For setting an initial size, representing the lenght of the top edge 
};

#endif

