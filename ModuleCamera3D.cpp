#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	name = "Camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, -5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	viewport_texture = new TextureMSAA(); 
	viewport_texture->Create(App->window->screen_surface->w, App->window->screen_surface->h, 2);

	SetSpeed(3); 

	start_time = performance_timer.Read();
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		float tmp_speed = GetSpeed();
		ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
		App->camera->SetSpeed(tmp_speed);
	}
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	performance_timer.Start();

	if (locked == true)
		return update_status::UPDATE_CONTINUE;

	vec3 point_look = { 0, 0, 0};
	int scale_value = 3; 
	bool moved = false; 
	
	//Look(Position, point_look, true); //Si descomentas aixo mirara tot el rato el punt que li diguis
	CalculateViewMatrix(); 

	//Camera WASD & ER input
	vec3 increment = { 0.0f ,0.0f ,0.0f }; 

	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		increment += Z * -GetSpeed()*App->GetDt();
		moved = true; 
	}

	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		increment += Z * GetSpeed() * App->GetDt();
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		increment += X * -GetSpeed() * App->GetDt();
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		increment += X * GetSpeed() * App->GetDt();
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		increment += Y * GetSpeed() * App->GetDt();
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_R))
	{
		increment += Y * -GetSpeed() * App->GetDt();
		moved = true;
	}

	if (moved)
		Move(increment);


	ManageMsBuffer();
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::SetSpeed(float new_speed)
{
	speed = new_speed; 
}

float ModuleCamera3D::GetSpeed()
{
	return speed;
}

TextureMSAA * ModuleCamera3D::GetViewportTexture()
{
	return viewport_texture;
}

void ModuleCamera3D::LockCamera()
{
	locked = true; 
}

void ModuleCamera3D::UnlockCamera()
{
	locked = false; 
}

bool ModuleCamera3D::IsLocked()
{
	return locked;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}