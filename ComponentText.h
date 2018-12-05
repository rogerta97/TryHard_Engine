#pragma once
#include "Component.h"

class UI_Label; 

class ComponentText : public Component
{
public:
	ComponentText();
	~ComponentText();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	UI_Label* GetLabel() const;

private:
	UI_Label * label;
};

