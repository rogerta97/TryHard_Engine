#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ComponentMesh.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	name = "Camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(3.0f, 3.0f, -8.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	orbit = true;
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
	SetMouseSensitivity(0.25f);

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
		float tmp_sensitivity = GetMouseSensitivity();
		ImGui::SliderFloat("Rotation", &tmp_sensitivity, 0.01f, 1.0f, "%.2f");
		ImGui::SameLine(); App->imgui->ShowHelpMarker("Hold the right mouse button\n" "and drag to rotate the camera.\n");
		App->camera->SetMouseSensitivity(tmp_sensitivity);
	}
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (locked == true)
		return update_status::UPDATE_CONTINUE;

	vec3 point_look = { 0, 0, 0};
	int scale_value = 3; 
	bool moved = false; 
	
	//Look(Position, point_look, true); //Si descomentas aixo mirara tot el rato el punt que li diguis

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

	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		LookAtSelectedGameObject(); 
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		orbit = !orbit;
	}


	if (moved)
		Move(increment);

	//// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dy != 0 || dx != 0) {

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * mouse_sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}



			if (dy != 0)
			{
				float DeltaY = (float)dy * mouse_sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			//CONSOLE_LOG("dy:%d dx:%d", dy, dx);


			if (orbit)
				Position = Reference + Z * length(Position);
		}
	}

	CalculateViewMatrix();

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

float3 ModuleCamera3D::GetCamPointFromDistance(vec center, float distance)
{
	float3 cam_pos(Position.x, Position.y, Position.z);

	LineSegment line;

	line.a = center;
	line.b = cam_pos;

	float equivalence = distance / line.Length();
	float3 dst_point = line.GetPoint(equivalence);

	return dst_point; 
}

void ModuleCamera3D::LookAtSelectedGameObject()
{
	//If it does not have childs just look at the AABB

	GameObject* selected_go = App->scene->GetSelectedGameObject(); 

	if (selected_go != nullptr)
	{
		ComponentMesh* cmp_mesh = (ComponentMesh*)App->scene->GetSelectedGameObject()->GetComponent(CMP_RENDERER);

		if (selected_go->GetNumChilds() == 0)
		{
			float distance = cmp_mesh->bounding_box.Diagonal().Length();

			//Rotate the camera
			const vec3 center(cmp_mesh->bounding_box.CenterPoint().x, cmp_mesh->bounding_box.CenterPoint().y, cmp_mesh->bounding_box.CenterPoint().z);

			//Get the segment from camera to center
			float3 dst_point = GetCamPointFromDistance(cmp_mesh->bounding_box.CenterPoint(), distance);
			Position.x = dst_point.x; Position.y = dst_point.y; Position.z = dst_point.z;
			LookAt(center);
			
		}
		else //if not find the middle point between the object and look at it. 
		{
			float3 pos_amm = {0,0,0};
			float dist_amm = 0; 

			selected_go->SetCenterCamDataRecursive(pos_amm, dist_amm); 

			float3 tmpcenter = pos_amm / App->scene->GetGameObjectsAmmount();

			vec center(tmpcenter.x, tmpcenter.y, tmpcenter.z);
			
			float3 dst_point = GetCamPointFromDistance(center, dist_amm);
			Position.x = dst_point.x; Position.y = dst_point.y; Position.z = dst_point.z;

			const vec3 center_ts(center.x, center.y, center.z);
			LookAt(center_ts);
		}
	}
	
}

void ModuleCamera3D::SetSpeed(float new_speed)
{
	speed = new_speed; 
}

void ModuleCamera3D::SetMouseSensitivity(float new_sensitivity)
{
	mouse_sensitivity = new_sensitivity; 
}

float ModuleCamera3D::GetMouseSensitivity() const
{
	return mouse_sensitivity;
}

float ModuleCamera3D::GetSpeed() const
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