#include "Application.h"

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
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->CleanUp();
		}
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}