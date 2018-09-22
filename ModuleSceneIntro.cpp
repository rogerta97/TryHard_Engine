#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "OpenGL.h"

#include "GameObject.h"

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

	GameObject* new_go = CreateGameObject(); 
	ComponentMesh* cmp = (ComponentMesh*)new_go->CreateComponent(CMP_RENDERER);
	cmp->SetMesh(App->resources->mesh_importer->GetMeshByType(MESH_PLANE)); 
	new_go->AddComponent(cmp); 

	AddGameObjectToScene(new_go); 

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject * ModuleSceneIntro::CreateGameObject()
{
	GameObject* new_go = new GameObject("plane_test"); 

	//Here we will add a transform component since it's mandatory

	return new_go; 
}

void ModuleSceneIntro::AddGameObjectToScene(GameObject* go)
{
	scene_gameobjects.push_back(go); 
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

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr)
		{
			(*it)->Update(); 
		}
	}

	//glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

	//GLfloat v0[3] = { 1.0f, 0.0f, 1.0f };
	//GLfloat v1[3] = { -1.0f, 0.0f, 1.0f };
	//GLfloat v2[3] = { 1.0f, 0.0f, -1.0f };
	//GLfloat v3[3] = { -1.0f, 0.0f, -1.0f };
	//						// front face =================
	//glVertex3fv(v1);    // v0-v1-v2
	//glVertex3fv(v0);
	//glVertex3fv(v2);

	//glVertex3fv(v3);    // v2-v3-v0
	//glVertex3fv(v1);
	//glVertex3fv(v2);

	//// right face =================
	//glVertex3fv(v0);    // v0-v3-v4
	//glVertex3fv(v3);
	//glVertex3fv(v4);

	//glVertex3fv(v4);    // v4-v5-v0
	//glVertex3fv(v5);
	//glVertex3fv(v0);

	//// top face ===================
	//glVertex3fv(v0);    // v0-v5-v6
	//glVertex3fv(v5);
	//glVertex3fv(v6);

	//glVertex3fv(v6);    // v6-v1-v0
	//glVertex3fv(v1);
	//glVertex3fv(v0);

	//glEnd(); 
	 
	return UPDATE_CONTINUE;
}



