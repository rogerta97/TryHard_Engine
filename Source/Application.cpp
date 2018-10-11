#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "DeviceId\DeviceId.h"
#include "JSON\parson.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio(true);
	imgui = new ModuleImGui();
	scene = new ModuleSceneIntro();
	resources = new ModuleResources();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	file_system = new ModuleFileSystem();
	//physics = new ModulePhysics3D(this);
	//player = new ModulePlayer(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules

	AddModule(window);

	AddModule(input);
	AddModule(camera);
	AddModule(file_system);
	AddModule(resources);
	AddModule(audio);
	//AddModule(physics);

	// Scenes
	AddModule(scene);

	// Renderer last!	
	AddModule(imgui);
	AddModule(renderer3D);


}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	//Load title name from globals
	name = TITLE;
	UpdateAppName();

	JSON_Object* config = json_value_get_object(json_parse_file("user-config.json"));

	if (!json_object_get_object(config, "App"))
		config = json_value_get_object(json_parse_file("config.json"));

	framerate_buffer.resize(HISTOGRAM_FR_LENGHT);

	for (int i = 0; i < framerate_buffer.size(); i++)
		framerate_buffer[i] = 0;

	org = "Concha La Lora";

	// Call Init() in all modules
	std::list<Module*>::iterator module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end() && ret == true)
	{
		(*module_iterator)->performance_timer.Start();
		ret = (*module_iterator)->Init(json_object_get_object(config, (*module_iterator)->name));
		module_iterator++;
	}

	// Apply config to app

	config = json_object_get_object(config, "App");

	vsync.is_active = false;
	vsync.vsync_lvl = 0;

	cap_fps = json_object_get_boolean(config, "cap_fps");
	if (cap_fps)
		max_fps = json_object_get_number(config, "max_fps");

	ImGui::GetStyle().FrameRounding = 15;

	// After all Init calls we call Start() in all modules
	CONSOLE_LOG("Application Start --------------");
	module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end() && ret == true)
	{
		(*module_iterator)->performance_timer.Start();
		ret = (*module_iterator)->Start();
		module_iterator++;
	}

	frame_delay = 0;
	frame_wish_time = 1.0f / max_fps;
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)frame_time.Read() / 1000;

	if (dt < frame_wish_time)
	{
		frame_delay = (frame_wish_time - dt) * 1000;
		SDL_Delay(frame_delay);
	}

	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	++frame_count;
	++last_sec_frame_count;

	ms_buffer.push_back(frame_time.Read());
	if (ms_buffer.size() > HISTOGRAM_MS_LENGHT)
		ms_buffer.erase(ms_buffer.begin());


	bool ret = true;

	if (last_sec_frame_time.Read() > 1000) {
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;

		framerate_buffer.push_back(prev_last_sec_frame_count);
		if (framerate_buffer.size() > HISTOGRAM_FR_LENGHT)
			framerate_buffer.erase(framerate_buffer.begin());


	}

	avg_fps = float(frame_count) / (startup_time.Read() / 1000);
	float seconds_since_startup = (startup_time.Read() / 1000);
	unsigned int last_frame_ms = frame_time.Read();
	float frames_on_last_update = prev_last_sec_frame_count;

	if (save_config_later) {
		SaveConfigData();
		save_config_later = false;
	}


}

void Application::GetHardWareData()
{
	uint64_t tot_vm, curr_vm, av_vm, res_vm;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &tot_vm, &curr_vm, &av_vm, &res_vm))
	{
		total_video_mem = (float)tot_vm / (1024.0f * 1024.0f);
		current_video_mem = (float)curr_vm / (1024.0f * 1024.0f);
		available_video_mem = (float)av_vm / (1024.0f * 1024.0f);
		reserved_video_mem = (float)res_vm / (1024.0f * 1024.0f);
	}
}

void Application::DisplayHardwareData()
{
	if (ImGui::CollapsingHeader("HardWare"))
	{
		GetHardWareData();

		SDL_version version;
		SDL_GetVersion(&version);

		ImGui::Text("SDL Version"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
		ImGui::Separator();
		ImGui::Text("CPU's:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", SDL_GetCPUCount()); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

		ImGui::Text("System RAM:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", SDL_GetSystemRAM());

		ImGui::Text("Caps: "); ImGui::SameLine();

		if (SDL_Has3DNow()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "3DNow,"); ImGui::SameLine();
		if (SDL_HasAVX()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX,"); ImGui::SameLine();
		//if (SDL_HasAVX2()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX2,"); ImGui::SameLine();		 Uncomment and the engine will crash 
		if (SDL_HasAltiVec()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AltiVec,"); ImGui::SameLine();
		if (SDL_HasMMX()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "MMX,"); ImGui::SameLine();
		if (SDL_HasRDTSC()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "RDTSC,"); ImGui::SameLine();
		if (SDL_HasSSE()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE,"); ImGui::SameLine();
		if (SDL_HasSSE2()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE2,"); ImGui::SameLine();
		if (SDL_HasSSE3()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE3,"); ImGui::SameLine();
		if (SDL_HasSSE41()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,"); ImGui::SameLine();
		if (SDL_HasSSE42()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE42,");

		ImGui::Separator();

		ImGui::Text("GPU Vendor: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsVendor());

		ImGui::Text("GPU Model: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsModel());

		ImGui::Separator();

		ImGui::Text("Total VRAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", total_video_mem);

		ImGui::Text("Using VRAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", current_video_mem);

		ImGui::Text("Avaliable VRAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", available_video_mem);

		ImGui::Text("Reserved VRAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", reserved_video_mem);

		{
			char buf[32];
			sprintf(buf, "%.2f/%.2f", current_video_mem, available_video_mem);
			float percentage = (current_video_mem * 100) / available_video_mem;

			ImGui::ProgressBar(percentage / 100, ImVec2(0.f, 0.f), buf);
			ImGui::SameLine();
			ImGui::Text("VRAM usage");
		}
	}
}

void Application::SaveConfigData()
{
	JSON_Value* config = json_parse_file("user-config.json");

	JSON_Value* app_config = json_value_init_object();

	JSON_Object* app_config_object = json_object(app_config);

	json_object_set_number(app_config_object, "max_fps", max_fps);
	json_object_set_boolean(app_config_object, "cap_fps", cap_fps);
	json_object_set_string(app_config_object, "engine_name", "");
	json_object_set_string(app_config_object, "organization", "");

	json_object_set_value(json_object(config), "App", app_config);

	std::list<Module*>::iterator module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end())
	{
		JSON_Value* module_config = json_value_init_object();
		(*module_iterator)->SaveConfigData(json_object(module_config));
		json_object_set_value(json_object(config), (*module_iterator)->name, module_config);
		module_iterator++;
	}
	json_serialize_to_file(config, "user-config.json");

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->performance_timer.Start();
		ret = (*item)->PreUpdate(dt);
		(*item)->ManageMsBuffer(TYPE_PREUPDATE);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->performance_timer.Start();
		ret = (*item)->Update(dt);
		(*item)->ManageMsBuffer(TYPE_UPDATE);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		(*item)->performance_timer.Start();
		ret = (*item)->PostUpdate(dt);
		(*item)->ManageMsBuffer(TYPE_POSTUPDATE);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	framerate_buffer.clear();
	ms_buffer.clear();

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->CleanUp();
		}
	}

	return ret;
}

Module * Application::GetModuleAt(int id)
{
	int i = 0;

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (i == id)
			return (*item);

		i++;
	}


	return nullptr;
}

void Application::DisplayConfigData()
{
	DisplayHardwareData();
	bool is_out = true;

	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Spacing();

		ImGui::InputText("Engine name", (char*)name.c_str(), name.size());
		UpdateAppName();

		//if (ImGui::IsInputTextFocused())
		//{
		//	App->camera->LockCamera(); 
		//	is_out = false; 
		//}

		ImGui::InputText("Organization", (char*)org.c_str(), org.size());

		//if (ImGui::IsInputTextFocused())
		//{
		//	App->camera->LockCamera();
		//	is_out = false; 
		//}

		if (is_out && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->camera->UnlockCamera();
		}

		ImGui::Spacing();

		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::Text("Framerate AVG: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", avg_fps);

		ImGui::SameLine();

		ImGui::Checkbox("Cap FPS", &cap_fps); ImGui::SameLine();

		ImGui::Checkbox("Vsync", &vsync.is_active);

		if (vsync.is_active)
		{
			//Use Vsync		
			if (GetLastSecFramerate() > 60)
				vsync.SetLevel(1);
			if (GetLastSecFramerate() < 60 && GetLastSecFramerate() > 30)
				vsync.SetLevel(2);
			else if (GetLastSecFramerate() < 30 && GetLastSecFramerate() > 16)
				vsync.SetLevel(3);
		}
		else
			vsync.SetLevel(0);

		if (VSYNC && SDL_GL_SetSwapInterval(vsync.vsync_lvl) < 0)
			CONSOLE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		if (cap_fps)
		{
			ImGui::Spacing();

			ImGui::DragInt("Cap Value", (int*)&max_fps, 1, 1, 1000);

			frame_wish_time = 1.0f / max_fps;

			ImGui::Spacing();
		}
		else {
			max_fps = 1000;

			frame_wish_time = 1.0f / max_fps;
		}

		ImGui::GetStyle().FrameRounding = 0;

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", framerate_buffer[framerate_buffer.size() - 1]);

		float highest = getHighest(framerate_buffer);
		float average = getAverage(framerate_buffer);

		int plot_margin = 10;

		ImGui::PlotHistogram("##Framerate", &framerate_buffer[0], HISTOGRAM_FR_LENGHT, 0, title, 0.0f,(highest-average)+average + highest * 0.1f, ImVec2(size.x, 100));

		sprintf_s(title, 25, "Miliseconds %.1f", ms_buffer[ms_buffer.size() - 1]);

		average = getAverage(ms_buffer);
		highest = getHighest(ms_buffer);


		ImGui::PlotHistogram("##Framerate", &ms_buffer[0], ms_buffer.size(), 0, title, 0.0f, (highest - average) + average + highest * 0.1f, ImVec2(size.x, 100));

		ImGui::GetStyle().FrameRounding = 80;
	}


}

void Application::UpdateAppName()
{
	SDL_SetWindowTitle(App->window->window, name.c_str());
}

void Application::OpenWebBrowser(const char * web)
{
	ShellExecute(NULL, "open", web, NULL, NULL, SW_SHOW);
}


const std::list<Module*>* Application::getModuleList() const
{
	return &list_modules;
}

float Application::GetDt() const
{
	return dt;
}

float Application::GetLastFrameDelay() const
{
	return frame_delay;
}

Vsync Application::GetVsync() const
{
	return vsync;
}

float Application::GetLastSecFramerate()
{
	return framerate_buffer[framerate_buffer.size() - 1];
}

void Application::SaveConfigAfterUpdate()
{
	save_config_later = true;
}

float Application::getHighest(std::vector<float> buffer)
{
	float highest = buffer[0];
	for (int i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] > highest)
			highest = buffer[i];
	}
	return highest;
}

float Application::getAverage(std::vector<float> buffer)
{
	float average = 0;
	int count = 0;
	for (int i = 0; i < buffer.size(); i++)
	{
		if (buffer[i] != 0) {
			average += buffer[i];
			count++;
		}
	}

	average /= count;

	return average;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}