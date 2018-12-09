#pragma once
#include "Module.h"
#include <map>
#include <string>
#include <functional>

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

	void FillFunctionList(std::string path);
	
	//std::map<const char*, std::function<TYPE>> function_list;
};

