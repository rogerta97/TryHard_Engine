#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "GameObject.h"

#include "ModuleFileSystem.h"

#include "ComponentMaterial.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"


#include "mmgr\mmgr.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled)
{
	name = "Input";
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(JSON_Object* config)
{
	CONSOLE_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	mouse_wheel = 0;

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		CONSOLE_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	file_droped = "";
	init_time = performance_timer.Read();
	return ret;
}

void ModuleInput::PrintConfigData()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "X: %d", App->input->mouse_x);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Y: %d", App->input->mouse_y);
		ImGui::Text("Mouse Motion:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "X: %d", App->input->mouse_x_motion);

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Y: %d", App->input->mouse_y_motion);

		ImGui::Text("Mouse Wheel:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "State: %d", App->input->mouse_wheel);

		ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (%.02f secs)", i, io.KeysDownDuration[i]); }
		ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", i); }
		ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", i); }
		ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
	}
}

std::string ModuleInput::GetFileDroped()
{
	return file_droped;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->scale;
	mouse_y /= App->window->scale;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;
	mouse_wheel = 0;

	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		App->imgui->SendInput(&e);

		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			mouse_wheel = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->width;
			mouse_y = e.motion.y / App->window->height;

			mouse_x_motion = e.motion.xrel / App->window->scale;
			mouse_y_motion = e.motion.yrel / App->window->scale;
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_DROPFILE:
		{
			file_droped = e.drop.file;
			App->BroadCastEvent(Event(FILE_DROPED)); 
		}

		break;

		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				App->renderer3D->OnResize(e.window.data1, e.window.data2);
		}
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}