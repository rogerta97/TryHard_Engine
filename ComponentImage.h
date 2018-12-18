#pragma once
#include "Component.h"
#include "UI_Image.h"

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* go);
	~ComponentImage();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor); 

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	UI_Image* GetImage() const; 
	void FitToRect(); 

private:
	UI_Image * image; 
};

