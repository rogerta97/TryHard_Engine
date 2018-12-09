#include "ModuleScripting.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include <fstream>


ModuleScripting::ModuleScripting()
{
}


ModuleScripting::~ModuleScripting()
{
}

bool ModuleScripting::Init(JSON_Object * config)
{
	return true;
}

bool ModuleScripting::Start()
{

	return true;
}

update_status ModuleScripting::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleScripting::CleanUp()
{
	return true;
}

void ModuleScripting::FillFunctionList(std::string path)
{
	//for (auto it = function_list.begin(); it != function_string_list.end(); it++)
	//{
	//	std::function<std::string> to_do;
	//	to_do = (*it).second; 
	//}
	//string directory = App->file_system->GetScriptsPath(); 
	//string file_name = "SimulatedScript.json"; 

	//if (!App->file_system->IsFileInDirectory(directory, file_name.c_str()))
	//{
	//	//Create Simulated Script File 
	//	string script_path = directory + string("\\") + file_name;

	//	std::ofstream stream;
	//	stream.open(script_path, std::fstream::out);

	//	JSON_Value* scene_v = json_value_init_object();
	//	JSON_Object* scene_obj = json_value_get_object(scene_v);

	//	//Save Meta Info
	//	json_object_dotset_number(scene_obj, "MetaInfo.UID", scene_to_save->GetUID());

	//	json_serialize_to_file(scene_v, item_meta_path.c_str());
	//}
}
