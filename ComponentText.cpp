#include "ComponentText.h"



ComponentText::ComponentText()
{
}


ComponentText::~ComponentText()
{
}

bool ComponentText::Start()
{
	return false;
}

bool ComponentText::Update()
{
	return false;
}

bool ComponentText::CleanUp()
{
	return false;
}

void ComponentText::Draw(bool is_editor)
{
}

UI_Label * ComponentText::GetLabel() const
{
	return label;
}
