#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "DeviceId\DeviceId.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio(true);
	imgui = new ModuleImGui();
	scene_intro = new ModuleSceneIntro();
	resources = new ModuleResources(); 
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	//physics = new ModulePhysics3D(this);
	//player = new ModulePlayer(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules

	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(resources); 
	//AddModule(audio);
	//AddModule(physics);

	// Scenes
	AddModule(scene_intro);

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

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = frame_time.Read() / 1000;
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	++frame_count;
	++last_sec_frame_count;

	bool ret = true;

	if (last_sec_frame_time.Read() > 1000) {
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / (startup_time.Read()/1000);
	float seconds_since_startup = (startup_time.Read()/1000);
	unsigned int last_frame_ms = frame_time.Read();
	float frames_on_last_update = prev_last_sec_frame_count;

	framerate_buffer.push_back(frames_on_last_update);
	if (framerate_buffer.size() > HISTOGRAM_FR_LENGHT)
		framerate_buffer.erase(framerate_buffer.begin());

	ms_buffer.push_back(last_frame_ms);
	if (ms_buffer.size() > HISTOGRAM_MS_LENGHT)
		ms_buffer.erase(ms_buffer.begin());
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

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
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

	if (ImGui::CollapsingHeader("Application"))
	{
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::Text("Framerate AVG: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", avg_fps);


		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", framerate_buffer[framerate_buffer.size() - 1]);
		ImGui::PlotHistogram("##Framerate", &framerate_buffer[0], framerate_buffer.size(), 0, title, 0.0f, 150.0f, ImVec2(size.x, 100));

		sprintf_s(title, 25, "Miliseconds %.1f", ms_buffer[ms_buffer.size() - 1]);
		ImGui::PlotHistogram("##Framerate", &ms_buffer[0], ms_buffer.size(), 0, title, 0.0f, 150.0f, ImVec2(size.x, 100));
	}


}

void Application::AddModule(Module* mod)  
{
	list_modules.push_back(mod);
}