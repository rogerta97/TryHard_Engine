#include "Globals.h"
#include "Application.h"
#include "imgui_dock.h"
#include "ModuleWindow.h"


ModuleWindow::ModuleWindow(bool start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* config)
{
	CONSOLE_LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		CONSOLE_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		scale = json_object_get_number(config, "scale");
		width = json_object_get_number(config,"width") * scale;
		height = json_object_get_number(config, "height") * scale;
		size_modified = false; 	
		brightness = 1.0f; 

		resizable = WIN_RESIZABLE; 
		borderless = WIN_BORDERLESS;

	
		if (json_object_get_boolean(config, "fullscreen"))
			display_mode = DISPLAY_FULLSCREEN_WINDOWED;
		else
			display_mode = DISPLAY_WINDOWED;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(display_mode == DISPLAY_FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(display_mode == DISPLAY_FULLSCREEN_WINDOWED)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

			SDL_DisplayMode mode; 
			SDL_GetDesktopDisplayMode(0, &mode);
			SDL_SetWindowPosition(window, 0, 0);
			ResizeWindow(mode.w, mode.h);
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		SDL_SetWindowBordered(window, (SDL_bool)false);

		if(window == NULL)
		{
			CONSOLE_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	init_time = performance_timer.Read();
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	CONSOLE_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::PrintConfigData()
{
	if(ImGui::CollapsingHeader("Window"))
	{
		if (size_modified)
			ResizeWindow(width, height);
		
		if (ImGui::DragInt("Width", &width, 1, 1, 10000))
			size_modified = true;

		if(ImGui::DragInt("Height", &height, 1, 1, 10000))
			size_modified = true;	

		ImGui::Separator();

		int dm = display_mode; 
		if (ImGui::Combo("Display Mode", &dm, "Fullscreen\0Fullscreen Windowed\0Windowed"))
		{
			Uint32 flags; 

			switch (dm)
			{
			case DISPLAY_FULLSCREEN:
				//It looks like shit we should work on resolution
				flags |= SDL_WINDOW_FULLSCREEN;
				SDL_SetWindowFullscreen(window, flags); 
				break; 

			case DISPLAY_FULLSCREEN_WINDOWED:
				SDL_DisplayMode mode; 
				SDL_GetDesktopDisplayMode(0, &mode);
				SDL_SetWindowPosition(window, 0, 0); 
				ResizeWindow(mode.w, mode.h); 
				break;

			case DISPLAY_WINDOWED:
				SDL_SetWindowPosition(window, 400, 100);
				ResizeWindow(1200, 1200);
				break;
			}
			display_mode = (Display_Mode)dm; 
		}

		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f, "%.2f"))
		{
			SDL_SetWindowBrightness(window, brightness); 
		}

		if (ImGui::Checkbox("Borderless", &borderless))
		{				
			SDL_SetWindowBordered(window, (SDL_bool)!borderless);
		} ImGui::SameLine(); 

		if (ImGui::Checkbox("Resizable", &resizable))
		{
	
		}
	}
}

void ModuleWindow::ResizeWindow(int new_width, int new_height)
{
	SDL_SetWindowSize(window, new_width, new_height); 
	width = new_width; 
	height = new_height;
}

int ModuleWindow::GetWidth() const
{
	return width;
}

int ModuleWindow::GetHeight() const
{
	return height;
}

void ModuleWindow::SaveConfigData(JSON_Object * config)
{
	json_object_set_number(config, "width", width);
	json_object_set_number(config, "height", height);
	json_object_set_number(config, "scale", 1);
	json_object_set_boolean(config, "fullscreen", false);
}
