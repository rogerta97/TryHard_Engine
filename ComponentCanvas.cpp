#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Application.h"
#include "UI_Canvas.h"

ComponentCanvas::ComponentCanvas(GameObject* parent)
{
	component_type = CMP_CANVAS;
	gameobject = parent; 
}


ComponentCanvas::~ComponentCanvas()
{
	delete canvas; 
}

bool ComponentCanvas::Start()
{

	canvas = new UI_Canvas();
	App->user_interface->AddCanvas(gameobject); 

	return false;
}

bool ComponentCanvas::Update()
{
	canvas->Update();
	return true;
}

bool ComponentCanvas::CleanUp()
{
	App->user_interface->DeleteCanvas(gameobject);
	return true;
}

void ComponentCanvas::Draw(bool is_editor)
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (auto it = canvas->elements_in_canvas.begin(); it != canvas->elements_in_canvas.end(); it++)
	{
		(*it)->Draw(is_editor);
	}
}

void ComponentCanvas::Load(JSON_Object * json_obj)
{
}

void ComponentCanvas::Save(JSON_Object * json_obj, const char * root)
{
	std::string item_name = root + std::string(".ComponentCanvas");

	int elements_num = canvas->elements_in_canvas.size();
	item_name += ".ObjectNum";
	json_object_dotset_number(json_obj, item_name.c_str(), elements_num);

	int i = 0; 
	for (auto it = canvas->elements_in_canvas.begin(); it != canvas->elements_in_canvas.end(); it++)
	{
		(*it)->Save(json_obj, i); 
	}
}

void ComponentCanvas::AddElement(GameObject * new_element)
{
	if (new_element != nullptr)
		canvas->elements_in_canvas.push_back(new_element); 
	else
	{
		CONSOLE_ERROR("Element Trying to be added is NULLPTR"); 
	}
}

UI_Element * ComponentCanvas::GetElement(std::string name) const
{
	return nullptr;
}

UI_Element * ComponentCanvas::GetElement(uint element_id) const
{
	return nullptr;
}

UI_Canvas * ComponentCanvas::GetCanvas() const
{
	return canvas;
}

void ComponentCanvas::SetCanvas(UI_Canvas * new_cnv)
{
	canvas = new_cnv; 
}



