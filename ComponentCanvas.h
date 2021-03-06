#ifndef _COMPONENT_CANVAS_H_
#define _COMPONENT_CANVAS_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_rect.h"

#include <list>
#include <string>

class UI_Element; 
class UI_Canvas; 

class ComponentCanvas : public Component
{
public:

	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor); 
	void OnEvent(const Event& new_event);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	void SetRenderElements(const bool& newValue);

	//Utility functions -----

	void AddElement(GameObject* new_element); 
	UI_Element* GetElement(std::string name) const;
	UI_Element* GetElement(uint element_id) const;

	UI_Canvas* GetCanvas() const; 
	void SetCanvas(UI_Canvas* new_cnv); 

	// ---------------------- 

private:

	UI_Canvas * canvas; 

	bool render_elements = true; 
};

#endif

