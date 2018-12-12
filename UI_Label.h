#pragma once
#include "UI_Element.h"

#include <string>
#include <list>
#include <vector>
#include <map>

#include "ModuleUserInterface.h"
#include "ComponentText.h"
#include "Font.h"

#include <map>

using namespace std; 

class ComponentText; 
class UI_Image; 

class UI_Label : public UI_Element
{
public:
	UI_Label(ComponentText* cmp_container);
	~UI_Label();

	void Start();
	void Update();
	void CleanUp();
	void Draw(bool is_editor);

	void FillTextPlanes();
	void CreateCharacterPlane(const char* character, float3 position);

	ComponentText* cmp_container; 
	float3 color;

	string GetText() const;
	void SetText(const char* new_text);
	void SetFont(string font_name);
	void ResizeFont(); 

	// Create container plane helpers
	void CreateEnclosedPlane(float3* points); 
	float3 GetValueFromRenderedText(const char* point); 
	float3 GetContainerPlanePoint(ClipTextType clipping);

	int text_size;

private: 

	string text;
	Font font;

	std::list<UI_Image*> text_planes; 
	std::vector<float3> offset_planes; 
};

