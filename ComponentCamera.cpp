#include "ComponentCamera.h"
#include "TextureMSAA.h"
#include "Application.h"

#include "ComponentMesh.h"

ComponentCamera::ComponentCamera()
{
	//name = "Camera";

	component_type = CMP_CAMERA; 

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(3.0f, 3.0f, 8.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	orbit = true;
}

ComponentCamera::~ComponentCamera()
{

}

// -----------------------------------------------------------------
bool ComponentCamera::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	viewport_texture = new TextureMSAA();
	viewport_texture->Create(App->window->screen_surface->w, App->window->screen_surface->h, 2);

	SetSpeed(3);
	SetMouseSensitivity(0.25f);
	projection = Projection_Type::PROJ_PERSP; 

	cam_interpolation.Init();

	return ret;
}

// -----------------------------------------------------------------
bool ComponentCamera::CleanUp()
{
	LOG("Cleaning camera");



	return true;
}

// -----------------------------------------------------------------
bool ComponentCamera::Update()
{

	if (locked == true)
		return update_status::UPDATE_CONTINUE;

	

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ComponentCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ComponentCamera::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix;
}

float3 ComponentCamera::GetCamPointFromDistance(vec center, float distance) const
{
	float3 cam_pos(Position.x, Position.y, Position.z);

	LineSegment line;

	line.a = center;
	line.b = cam_pos;

	float equivalence = distance / line.Length();
	float3 dst_point = line.GetPoint(equivalence);

	return dst_point;
}

void ComponentCamera::FillInterpolationSegmentAndRot()
{
	//If it does not have childs just look at the AABB

	GameObject* selected_go = App->scene->GetSelectedGameObject();

	if (selected_go != nullptr)
	{
		ComponentMesh* cmp_mesh = (ComponentMesh*)App->scene->GetSelectedGameObject()->GetComponent(CMP_RENDERER);

		if (selected_go->GetNumChilds() == 0)
		{
			float distance = cmp_mesh->bounding_box.Diagonal().Length() + 1;

			vec3 center(cmp_mesh->bounding_box.CenterPoint().x, cmp_mesh->bounding_box.CenterPoint().y, cmp_mesh->bounding_box.CenterPoint().z);

			float3 dst_point = GetCamPointFromDistance(cmp_mesh->bounding_box.CenterPoint(), distance);

			cam_interpolation.line.a = dst_point;
			cam_interpolation.line.b = float3({ Position.x, Position.y, Position.z });

			cam_interpolation.dst_vec = -normalize(Position - center);
			cam_interpolation.source_vec = -Z;

			cam_interpolation.center = cmp_mesh->bounding_box.CenterPoint();

		}
		else //if not find the middle point between the object and look at it. 
		{
			float3 pos_amm = { 0,0,0 };
			float dist_amm = 0;

			selected_go->SetCenterCamDataRecursive(pos_amm, dist_amm);

			float3 tmpcenter = pos_amm / App->scene->GetGameObjectsAmmount();

			vec center(tmpcenter.x, tmpcenter.y, tmpcenter.z);

			float3 dst_point = GetCamPointFromDistance(center, dist_amm + 1);

			cam_interpolation.center = center;

			cam_interpolation.line.a = dst_point;
			cam_interpolation.line.b = float3({ Position.x, Position.y, Position.z });

			cam_interpolation.dst_vec = -normalize(Position - vec3(center.x, center.y, center.z));
			cam_interpolation.source_vec = -Z;
		}
	}
}

bool ComponentCamera::InterpolateCamera(float time)
{
	vec3 look_point = { 0,0,0 };

	vec3 center = { cam_interpolation.center.x, cam_interpolation.center.y, cam_interpolation.center.z };

	float3 src_vector = { cam_interpolation.source_vec.x,  cam_interpolation.source_vec.y , cam_interpolation.source_vec.z };
	float3 dst_vector = { cam_interpolation.dst_vec.x,  cam_interpolation.dst_vec.y , cam_interpolation.dst_vec.z };
	float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, 1);

	bool end = false;

	if (Reference.x == center.x && Reference.y == center.y && Reference.z == center.z) {
		if (dst_vector.x == curr_rot.x && dst_vector.y == curr_rot.y && dst_vector.z == curr_rot.z)
		{
			end = true;
		}
		if (dst_vector.x != src_vector.x || dst_vector.y != src_vector.y || dst_vector.z != src_vector.z)
		{
			end = false;
		}

	}

	if (end) {
		cam_interpolation.interpolate = false;
		return true;
	}

	if (cam_interpolation.interpolation_timer.Read() <= time)
	{
		float percentage = (float)cam_interpolation.interpolation_timer.Read() / time;

		//Interpolate Position
		Position = { cam_interpolation.line.GetPoint(1 - percentage).x, cam_interpolation.line.GetPoint(1 - percentage).y, cam_interpolation.line.GetPoint(1 - percentage).z };

		//Interpolate Rotation
		float3 src_vector = { cam_interpolation.source_vec.x,  cam_interpolation.source_vec.y , cam_interpolation.source_vec.z };
		float3 dst_vector = { cam_interpolation.dst_vec.x,  cam_interpolation.dst_vec.y , cam_interpolation.dst_vec.z };

		float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, percentage);

		look_point = { curr_rot.x, curr_rot.y, curr_rot.z };

		LookAt(Position + look_point);

		return false;
	}
	else
	{
		cam_interpolation.interpolate = false;
		LookAt(center);
		return true;
	}
}

void ComponentCamera::SetSpeed(float new_speed)
{
	speed = new_speed;
}

void ComponentCamera::SetMouseSensitivity(float new_sensitivity)
{
	mouse_sensitivity = new_sensitivity;
}

float ComponentCamera::GetMouseSensitivity() const
{
	return mouse_sensitivity;
}

float ComponentCamera::GetSpeed() const
{
	return speed;
}

TextureMSAA * ComponentCamera::GetViewportTexture()
{
	return viewport_texture; 
}

void ComponentCamera::LockCamera()
{
	locked = true;
}

void ComponentCamera::UnlockCamera()
{
	locked = false;
}

bool ComponentCamera::IsLocked() const
{
	return locked;
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
