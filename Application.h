#ifndef _APPLICATION_H_
#define _APPLICATION_H_


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
#include "ModuleUserInterface.h"
#include "ModuleScripting.h"
#include "ModuleFileSystem.h"
#include "ModuleTime.h"

#include <list>
#include <string>
#include <vector>

#define HISTOGRAM_FR_LENGHT 50
#define HISTOGRAM_MS_LENGHT 50
#define MAX_MEMORY_LOGGED	50

enum GameState {RUNNING, PAUSED, STOPPED};

class UI_Button; 

struct Vsync
{
	void SetActive(const bool& active)
	{
		is_active = active;	
	};

	bool GetActive()
	{
		return is_active; 
	}

	void ControlVsync()
	{
		if (is_active)
		{
			if (frame_time.Read() < 16)
			{
				int time_to_wait = 16 - frame_time.Read();
				SDL_Delay(time_to_wait);
			}

			else if (frame_time.Read() < 33)
			{
				int time_to_wait = 33 - frame_time.Read();
				SDL_Delay(time_to_wait);
			}
		}

		BeginTime(); 
	}

	void BeginTime()
	{
		frame_time.Start();
	}

private:

	bool is_active = false;
	Timer frame_time; 

};

enum EventType { PLAY, PAUSE, STOP, FILE_DROPED, RECTTRANSFORM_RESIZED, UI_ELEMENT_DOWN, UI_ELEMENT_UP, UI_ELEMENT_ENTER, UI_ELEMENT_OUT, INTERPOLATE_ALPHA
};

struct Event {
	EventType type;

	union
	{
		struct
		{
			const char* ptr;
		} string;
		struct
		{
			int x, y;
		} point2d;
		struct
		{
			UI_Button* but;

		} button;
		struct
		{
			float percentage;
		} alpha_lvl;
	};

	Event() {};
	Event(EventType type) :type(type) {};
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
	ModuleUserInterface* user_interface;
	ModuleTime* time_manager;
	ModuleScripting* script; 

	void SetGameState(const GameState& new_gs);
	GameState& GetGameState(); 

	void SetVsync(bool vsync_active);

private:
	
	GameState current_game_state = STOPPED;

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

	MEMORYSTATUSEX memInfo;

	Vsync				vsync; 
	
	std::list<Module*> list_modules;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void BroadCastEvent(const Event& event);

	Module* GetModuleAt(int id);
	void DisplayConfigData();

	void UpdateAppName(); 

	void OpenWebBrowser(const char* web); 
	const std::list<Module*>* getModuleList() const;

	float GetDt() const;
	float GetLastFrameDelay() const;
	Vsync GetVsync() const; 
	float GetLastSecFramerate() const; 
	void SaveConfigAfterUpdate();

	void CapFPS(int max); 

	void Play();
	void Pause();

	float getHighest(std::vector<float>	buffer);
	float getAverage(std::vector<float>	buffer);

	bool game_running;
	bool game_paused;


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

#endif