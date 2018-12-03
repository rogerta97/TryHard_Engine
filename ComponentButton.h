#pragma once
#include "Component.h"

class UI_button; 

class ComponentButton : public Component
{
public:
	ComponentButton(GameObject* parent);
	~ComponentButton();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	UI_button* GetButton() const;

private:
	UI_button * button;
};

