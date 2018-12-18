#pragma once
#include "Component.h"

#include "Transform.h"

enum Scale_Type {ST_CONSTANT,ST_SCREEN_SIZE};

class ComponentCanvasScaler : public Component
{
public:
	ComponentCanvasScaler(GameObject* parent);
	~ComponentCanvasScaler();

	bool Start();
	bool Update();
	bool CleanUp();
	void OnEvent(const Event& new_event);

	float GetScaleFactor(); 
	void SetScaleFactor(float newValue); 

	Scale_Type GetScaleType() const;
	void SetScaleType(Scale_Type new_type);

private:
	float scale_factor; 
	Scale_Type scale_type;
};

