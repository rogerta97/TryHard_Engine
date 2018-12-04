#pragma once
#include "UI_Element.h"
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std; 

class ComponentText; 

class UI_Label : public UI_Element
{
public:
	UI_Label(ComponentText* cmp_container);
	~UI_Label();

	void Start();
	void Update();
	void CleanUp();
	void Draw(bool is_editor);

	ComponentText* cmp_container; 

	void SetText(const char* new_text);

private: 

	string text; 

};

