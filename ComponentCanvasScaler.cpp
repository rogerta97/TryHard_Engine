#include "ComponentCanvasScaler.h"
#include "ComponentRectTransform.h"
#include "Application.h"
#include "UI_Canvas.h"



ComponentCanvasScaler::ComponentCanvasScaler(GameObject* parent)
{
	component_type = CMP_CANVASSCALER;
	gameobject = parent;
	scale_type = ST_CONSTANT;
}

ComponentCanvasScaler::~ComponentCanvasScaler()
{
}

bool ComponentCanvasScaler::Start()
{

	return true;
}

bool ComponentCanvasScaler::Update()
{
	return true;
}

bool ComponentCanvasScaler::CleanUp()
{
	return true;
}

float ComponentCanvasScaler::GetScaleFactor()
{
	return scale_factor;
}

void ComponentCanvasScaler::SetScaleFactor(float newValue)
{
	scale_factor = newValue; 
}

void ComponentCanvasScaler::SetScaleType(Scale_Type new_type)
{
	scale_type = new_type;
}

Scale_Type ComponentCanvasScaler::GetScaleType() const
{
	return scale_type;
}
