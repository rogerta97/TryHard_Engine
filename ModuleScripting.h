#pragma once
#include "Module.h"
#include <map>
#include <string>
#include <functional>

class UI_CallbackAgent; 

// This module is just a simulation, will read a file (script) and store functions in order to use them as callbacks in UI. 

class ModuleScripting : public Module
{
public:
	ModuleScripting();
	~ModuleScripting();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update();
	bool CleanUp();

	void FillFunctionList();

	void PrintFunctionsList(UI_CallbackAgent* agent, int index);
	
	std::map<const char*, std::function<void()>> function_list;
	std::map<const char*, std::function<void(const char*)>> function_string_list;
	std::map<const char*, std::function<void(bool)>> function_bool_list;
};

