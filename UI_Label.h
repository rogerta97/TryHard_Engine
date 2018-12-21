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

	ComponentText* cmp_container;
	
	void Start();
	void Update();
	void CleanUp();
	void Draw(bool is_editor);
	void RenderText();

	GameObject* GetParentGameObject();

	// Planes 
	void FillTextPlanes();												// Create the UI images with each corresponding size and texture following 'text'
	void CreateCharacterPlane(const char* character, float3 position);	// Creates the plane of a letter 

	//Text Origin
	float2 GetOrigin() const;
	void SetOrigin(const float2 new_origin);
	void TranslateOrigin(float2 increment);

	//Section
	void AdvanceSection(); 
	void RegressSection(); 

	float3 color;

	//Text Utility
	void CleanText(); 
	string GetText() const;
	void SetText(const char* new_text);
	void SetFont(string font_name);
	Font GetFont() const; 
	void ResizeFont(); 
	bool ControlNewLine(float3& cursor, std::vector<float3>& offset_planes, const ClipTextType clipping_type, int& current_line, const int counter, const float2 init_offset);												// Will cut the text and start to render in the line below if needed. 

	// Create container plane helpers

	void CreateEnclosedPlane(float3* points);							// Update Enclosed Plane (Delete and create a new one)
	float3 GetValueFromRenderedText(const char* point);					// Get highest/lowest x/y from the rendered text in world space
	float3 GetContainerPlanePoint(ClipTextType clipping);				// Get the clipping point in world space 
	void UpdateContainerPlane();										// Fits the container plane to the text, and clip it 

	//Setters & Getters
	std::vector<float3>& GetOffsetList();
	float3 GetOffsetListValue(const int& index);

	int text_size = 0;
	float2 section = {-1,-1};
	char inspector_text[255];

private: 			

	string text;
	Font font;

	float2 text_origin = {0,0};
	std::list<UI_Image*> text_planes; 
	std::vector<float3> offset_planes; 
};

