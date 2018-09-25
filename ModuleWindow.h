#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

enum Display_Mode
{
	DISPLAY_FULLSCREEN,
	DISPLAY_FULLSCREEN_WINDOWED,
	DISPLAY_WINDOWED
};

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* config);
	bool CleanUp();

	void SetTitle(const char* title);
	void PrintConfigData(); 

	void ResizeWindow(int new_width, int new_height);
	int GetWidth() const;
	int GetHeight() const;

	void SaveConfigData(JSON_Object* config);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	//To have them in case of resize
	int width, height;
	float scale;

private: 
	Display_Mode display_mode;
	bool borderless, resizable; 
	float brightness; 

	bool size_modified; 
};

#endif // __ModuleWindow_H__