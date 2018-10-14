#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleResources.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"

#include <list>
#include <string>
#include <vector>

#define HISTOGRAM_FR_LENGHT 50
#define HISTOGRAM_MS_LENGHT 50
#define MAX_MEMORY_LOGGED	50

struct Vsync
{
	bool	is_active = false;
	uint	vsync_lvl = 0;

	void SetActive(bool active) { is_active = active; };
	void SetLevel(uint lvl) { vsync_lvl = lvl; };
};

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleResources* resources; 
	ModuleImGui* imgui;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleFileSystem* file_system; 

private:
	
	std::string name;
	std::string org;

	Timer	ms_timer;

	Timer				last_sec_frame_time;
	unsigned int		last_sec_frame_count = 0;
	unsigned int		prev_last_sec_frame_count = 0;

	unsigned int		frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	float				dt = 0.0f;
	uint				max_fps = 1000;
	bool				cap_fps;
	float				frame_wish_time; 
	int                 frame_delay;
	float				avg_fps = -1.0f;

	Vsync				vsync; 
	
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	Module* GetModuleAt(int id);
	void DisplayConfigData();

	void UpdateAppName(); 

	void OpenWebBrowser(const char* web); 
	const std::list<Module*>* getModuleList() const;

	float GetDt() const;
	float GetLastFrameDelay() const;
	Vsync GetVsync() const; 
	float GetLastSecFramerate(); 
	void SaveConfigAfterUpdate();

	float getHighest(std::vector<float>	buffer);
	float getAverage(std::vector<float>	buffer);


private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void GetHardWareData();
	void DisplayHardwareData();
	void SaveConfigData();

	bool save_config_later = false;

private:

	//Hardware
	float total_video_mem = -1.0f;
	float current_video_mem = -1.0f;
	float available_video_mem = -1.0f;
	float reserved_video_mem = -1.0f;

	std::vector<float>	framerate_buffer;
	std::vector<float>	ms_buffer;
	std::vector<float>	memory;
};