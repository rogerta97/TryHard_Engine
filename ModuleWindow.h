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

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void PrintConfigData(); 

	void ResizeWindow(int new_width, int new_height);
	int GetWidth() const;
	int GetHeight() const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private: 

	int width, height; 
	Display_Mode display_mode;
	bool borderless; 

	bool size_modified; 
};

#endif // __ModuleWindow_H__