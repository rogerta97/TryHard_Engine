#pragma once
#include "Module.h"

enum file_extension
{
	FX_PNG,
	FX_FBX,
	FX_DDS,
	FX_ERR
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	file_extension GetFileExtension(std::string full_path);
	std::string GetModelsPath(); 

private:

	std::string models_path; 


};

