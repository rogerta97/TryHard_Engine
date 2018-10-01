#pragma once
#include "Module.h"

using namespace std; 

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

	file_extension GetFileExtension(string full_path);
	string GetWorkingDirectory() const;

	string GetModelsPath(); 
	string GetTexturesPath();

private:

	string game_path; 
	string models_path; 
	string textures_path;


};

