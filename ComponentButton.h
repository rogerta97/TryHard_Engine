#pragma once
#include "Component.h"

class UI_Button;

class ComponentButton : public Component
{
public:
	ComponentButton(GameObject* parent);
	~ComponentButton();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	UI_Button* GetButton() const;

private:
	UI_Button * button;
};

