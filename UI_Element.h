#pragma once

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
	void SetType(UI_Widgget_Type new_type);

	float GetPercentage() const;
	void SetPercentage(float new_num);

	bool GetRaycast() const;
	void SetRaycast(bool new_ray);


private:

	UI_Widgget_Type wid_type;	// Type of UI element 
	UI_Canvas* canvas;			// Canvas where the element lays
	bool raycast;				// Make the canvas clickable
	float canvas_percentage;	// For setting an initial size 
};

