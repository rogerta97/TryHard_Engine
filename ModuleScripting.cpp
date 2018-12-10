#include "ModuleScripting.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleScene.h"
#include "UICallbackSystem.h"

#include "ComponentButton.h"

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
		json_object_dotset_string(scene_obj, "Function1.name", "EnableWireframe");
		json_object_dotset_string(scene_obj, "Function1.args", "void");

		json_object_dotset_string(scene_obj, "Function2.name", "DisableWireframe");
		json_object_dotset_string(scene_obj, "Function2.args", "void");

		json_object_dotset_string(scene_obj, "Function3.name", "LoadScene");
		json_object_dotset_string(scene_obj, "Function3.args", "const char*");

		json_object_dotset_number(scene_obj, "Info.num", 3);

		json_serialize_to_file(scene_v, script_path.c_str());
	}

	std::ifstream stream;
	stream.open(script_path.c_str(), std::fstream::in);

	JSON_Value* root = json_parse_file(script_path.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int number = json_object_dotget_number(root_obj, "Info.num");

	// void functions ----------------------------------------------------------

	JSON_Object* parent_obj = root_obj;

	for (int i = 1; i <= number; i++)
	{
		string file_node_name = "Function" + to_string(i); 
		root_obj = json_object_get_object(parent_obj, file_node_name.c_str());

		string type = json_object_get_string(root_obj, "args");
		string func_name = json_object_get_string(root_obj, "name");

		if (type == "void")
		{
			if (func_name == "EnableWireframe")
			{
				std::function<void()> callback = []() {App->renderer3D->render_settings.EnableWireframe(); App->renderer3D->UseCurrentRenderSettings(); };
				function_list.insert(std::pair<const char*, std::function<void()>>("EnableWireframe", callback));
			}

			if (func_name == "DisableWireframe")
			{
				std::function<void()> callback = []() {App->renderer3D->render_settings.DisableWireframe();  App->renderer3D->UseCurrentRenderSettings(); };
				function_list.insert(std::pair<const char*, std::function<void()>>("DisableWireframe", callback));
			}
		}

		if (type == "const char*")
		{
			if (func_name == "LoadScene")
			{			
				/*std::function<void(const char*)> callback = App->scene->LoadScene;
				function_string_list.insert(std::pair<const char*, std::function<void(const char*)>>("CapFPS", callback));*/
			}
		}
	}

	stream.close();

}

void ModuleScripting::PrintFunctionsList(UI_CallbackAgent* agent, int index)
{
	if (ImGui::BeginPopup("select_callback"))
	{
		string label_name = "Void##" + to_string(index); 
		if (ImGui::BeginMenu(label_name.c_str()))
		{
			int i = 0;
			for (auto it = function_list.begin(); it != function_list.end(); it++, i++)
			{
				string curr_name = (*it).first;

				ImGui::Selectable(curr_name.c_str());

				if (ImGui::IsItemClicked(0))
				{
					agent->action = (*it).second; 
					agent->name = curr_name;

					ImGui::EndMenu();
					ImGui::EndPopup();
					agent->show_function_list = false; 

					ComponentButton* button_cmp = agent->system_container->GetSystemOwner();
					button_cmp->OnMousePressed.push_back((*it).second);

					return;
				}
			}

			ImGui::EndMenu();
		}
			
		label_name = "Char##" + to_string(index);
		if (ImGui::BeginMenu(label_name.c_str()))
		{
			int i = 0;
			for (auto it = function_string_list.begin(); it != function_string_list.end(); it++, i++)
			{
				string curr_name = (*it).first;

				if (ImGui::Selectable(curr_name.c_str()))
				{
					agent->action_char = (*it).second;
					agent->name = curr_name;

					ImGui::EndMenu();
					ImGui::EndPopup();
					agent->show_function_list = false;

					return;
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}
