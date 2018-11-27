#include "ComponentRectTransform.h"



ComponentRectTransform::ComponentRectTransform(GameObject* parent)
{
	component_type = CMP_RECTTRANSFORM;
}


ComponentRectTransform::~ComponentRectTransform()
{
}

bool ComponentRectTransform::Start()
{
	return false;
}

bool ComponentRectTransform::Update()
{
	return false;
}

bool ComponentRectTransform::CleanUp()
{
	return false;
}
