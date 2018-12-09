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
	FillFunctionList();

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

void ModuleScripting::FillFunctionList()
{
	string directory = App->file_system->GetScriptsPath(); 
	string file_name = "SimulatedScript.json"; 

	string script_path = directory + string("\\") + file_name;

	if (!App->file_system->IsFileInDirectory(directory, file_name.c_str()))
	{
		//Create Simulated Script File 
		std::ofstream stream;
		stream.open(script_path, std::fstream::out);

		JSON_Value* scene_v = json_value_init_object();
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		//Save Meta Info
		json_object_dotset_string(scene_obj, "FunctionNames.name_1", "EnableWireframe");
		json_object_dotset_string(scene_obj, "FunctionNames.name_2", "DisableWireframe");
		json_object_dotset_string(scene_obj, "FunctionNames.name_3", "LoadStreetScene");

		json_serialize_to_file(scene_v, script_path.c_str());
	}

	std::ifstream stream;
	stream.open(script_path.c_str(), std::fstream::in);

	JSON_Value* root = json_parse_file(script_path.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	string func_name = json_object_dotget_string(root_obj, "FunctionNames.name_1");

	if (func_name == "EnableWireframe")
	{
		std::function<void()> callback = []() {App->renderer3D->render_settings.EnableWireframe(); };
		function_list.insert(std::pair<const char*, std::function<void()>>("EnableWireframe", callback));
	}

	func_name = json_object_dotget_string(root_obj, "FunctionNames.name_2");

	if (func_name == "DisableWireframe")
	{
		std::function<void()> callback = []() {App->renderer3D->render_settings.DisableWireframe(); };
		function_list.insert(std::pair<const char*, std::function<void()>>("DisableWireframe", callback));
	}

	func_name = json_object_dotget_string(root_obj, "FunctionNames.name_3");

	if (func_name == "LoadStreetScene")
	{
		//std::function<void> callback = App->renderer3D->render_settings.EnableWireframe;
		//function_list.insert(std::pair<const char*, std::function<void()>>("EnableWireframe", callback));
	}
}
