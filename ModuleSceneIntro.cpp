#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"




ModuleSceneIntro::ModuleSceneIntro(bool start_enabled)
{

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
	//ImGui::Button("hello");

	
	
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
	p.color = { 0.8f,0.8f,0.6f };
	p.Render();

	Sphere s({0,0,0},100);

	Plane p2({ 0,0,0 }, 0);

	Circle intersects = s.Intersect(p2);
	 



	return UPDATE_CONTINUE;
}



