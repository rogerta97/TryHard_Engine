#include "ComponentCanvasScaler.h"



ComponentCanvasScaler::ComponentCanvasScaler()
{
}


ComponentCanvasScaler::~ComponentCanvasScaler()
{
}

bool ComponentCanvasScaler::Start()
{
	return false;
}

bool ComponentCanvasScaler::Update()
{
	return false;
}

bool ComponentCanvasScaler::CleanUp()
{
	return false;
}

float ComponentCanvasScaler::GetScaleFactor()
{
	return scale_factor;
}

void ComponentCanvasScaler::SetScaleFactor(float newValue)
{
	scale_factor = newValue; 
}
