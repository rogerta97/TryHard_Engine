#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = float3(0.0f, 0.0f, 0.0f);
	transform.scale = float3(0.0f, 0.0f, 0.0f);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Start()
{
}

void ComponentTransform::Update()
{
}
