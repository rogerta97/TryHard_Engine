#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"




ModuleSceneIntro::ModuleSceneIntro(bool start_enabled)
{
	name = "Scene";
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	start_time = performance_timer.Read();
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	pPlane p(0, 1, 0, 0);
	p.axis = true;
	p.color = { 233,0,255};  //Em mola que flipas el rosa de moment deixal porfa 
	p.Render();

	Sphere s({0,0,0},100);

	Plane p2({ 0,0,0 }, 0);

	Circle intersects = s.Intersect(p2);
	 
	return UPDATE_CONTINUE;
}



