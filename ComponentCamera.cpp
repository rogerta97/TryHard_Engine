#include "ComponentCamera.h"
#include "TextureMSAA.h"
#include "Application.h"

#include "ComponentMesh.h"

ComponentCamera::ComponentCamera()
{
	//name = "Camera";

	component_type = CMP_CAMERA; 

	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(3.0f, 3.0f, 8.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);


	horizontal_fov = 90;

	//frustum.horizontalFov = fov_x;
	frustum.horizontalFov = DegToRad(90);
	frustum.verticalFov = DegToRad(60);
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 512;
	//frustum.orthographicWidth = 1000;
	//frustum.orthographicHeight = 800;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.SetWorldMatrix(float3x4::identity);

	size.x = 1500;
	size.y = 1000;

	orbit = true;

	mouse_sensitivity = 0.01f;
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
	viewport_texture->Create(size.x, size.y, 2);

	//viewport_texture->SetHeight(size.y);
	//viewport_texture->SetWidth(size.x);

	SetSpeed(3);
	SetMouseSensitivity(0.01f);


	interpolation.Init();

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

	frustum.pos = Position;

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ComponentCamera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ComponentCamera::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix[0][0];
}

float3 ComponentCamera::GetCamPointFromDistance(float2 center, float distance) const
{
	//float3 cam_pos(Position.x, Position.y, Position.z);

	//LineSegment line;

	//line.a = center;
	//line.b = cam_pos;

	//float equivalence = distance / line.Length();
	//float3 dst_point = line.GetPoint(equivalence);

	//return dst_point;
	return { 0,0,0 };
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

			float3 center(cmp_mesh->bounding_box.CenterPoint().x, cmp_mesh->bounding_box.CenterPoint().y, cmp_mesh->bounding_box.CenterPoint().z);

			//float3 dst_point = GetCamPointFromDistance(cmp_mesh->bounding_box.CenterPoint(), distance);
			//
			//interpolation.line.a = dst_point;
			//interpolation.line.b = float3({ Position.x, Position.y, Position.z });

			//interpolation.dst_vec = -(Position - center).Normalized();
			//interpolation.source_vec = -Z;

			//interpolation.center = cmp_mesh->bounding_box.CenterPoint();
			
			

		}
		else //if not find the middle point between the object and look at it. 
		{
			//ComponentMesh* parent_mesh = (ComponentMesh*)selected_go->GetComponent(CMP_RENDERER);

			//float3 min, max, position;
			//float distance; 

			//min = max = position = { 0,0,0 };

			//selected_go->GetEnclosedAABB(min, max);
			//selected_go->SetCenterCamDataRecursive(position, distance);

			//AABB new_bb; 
			//new_bb.minPoint = min; 
			//new_bb.maxPoint = max; 
			//distance = new_bb.Diagonal().Length() + 1; 

			//float3 tmpcenter = position / App->scene->GetGameObjectsAmmount();
			//float2 center(tmpcenter.x, tmpcenter.y, tmpcenter.z);

			//float3 dst_point = GetCamPointFromDistance(center, distance);

			//interpolation.center = center;
			//
			//interpolation.line.a = dst_point;
			//interpolation.line.b = float3({ Position.x, Position.y, Position.z });
		
			//interpolation.dst_vec = -(Position - float3(center.x, center.y, center.z)).Normalized();
			//interpolation.source_vec = -Z;
		}
	}
}

bool ComponentCamera::InterpolateCamera(float time)
{
	//float3 look_point = { 0,0,0 };

	//float3 center = { interpolation.center.x, interpolation.center.y, interpolation.center.z };

	//float3 src_vector = { interpolation.source_vec.x,  interpolation.source_vec.y , interpolation.source_vec.z };
	//float3 dst_vector = { interpolation.dst_vec.x,  interpolation.dst_vec.y , interpolation.dst_vec.z };
	//float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, 1);

	//bool end = false;

	//if (Reference.x == center.x && Reference.y == center.y && Reference.z == center.z) {
	//	end = true;
	//	if (!AreFloat3Same(src_vector, dst_vector, 0.001f))
	//		end = false;
	//}

	//if (end) {
	//	interpolation.interpolate = false;
	//	return true;
	//}

	//if (interpolation.interpolation_timer.Read() <= time)
	//{
	//	float percentage = (float)interpolation.interpolation_timer.Read() / time;

	//	//Interpolate Position
	//	Position = { interpolation.line.GetPoint(1 - percentage).x, interpolation.line.GetPoint(1 - percentage).y, interpolation.line.GetPoint(1 - percentage).z };

	//	//Interpolate Rotation
	//	float3 src_vector = { interpolation.source_vec.x,  interpolation.source_vec.y , interpolation.source_vec.z };
	//	float3 dst_vector = { interpolation.dst_vec.x,  interpolation.dst_vec.y , interpolation.dst_vec.z };

	//	float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, percentage);

	//	look_point = { curr_rot.x, curr_rot.y, curr_rot.z };

	//	LookAt(Position + look_point);

	//	return false;
	//}
	//else
	//{
	//	interpolation.interpolate = false;
	//	LookAt(center);
	//	return true;
	//}
	return false;
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

Projection_Type ComponentCamera::GetProjection() const
{
	return projection;
}

void ComponentCamera::SetProjection(Projection_Type type)
{
	projection = type; 
}

Camera_Aspect ComponentCamera::GetAspect() const
{
	return aspect;
}

void ComponentCamera::SetAspect(Camera_Aspect type)
{
	aspect = type; 
}

float ComponentCamera::GetSpeed() const
{
	return speed;
}

void ComponentCamera::SetWidth()
{
}

float ComponentCamera::GetWidth() const
{
	return size.x;
}

void ComponentCamera::SetHeight()
{
}

float ComponentCamera::GetHeight() const
{
	return size.y;
}

TextureMSAA * ComponentCamera::GetViewportTexture()
{
	return viewport_texture; 
}

bool ComponentCamera::AreSame(float a, float b, float epsilon)
{
	return fabs(a - b) < epsilon;
}

bool ComponentCamera::AreFloat3Same(float3 a, float3 b, float epsilon)
{
	bool ret = true;
	if (!AreSame(a.x, b.x, epsilon))
		ret = false;
	if (!AreSame(a.y, b.y, epsilon))
		ret = false;
	if (!AreSame(a.z, b.z, epsilon))
		ret = false;

	return ret;
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
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f,
		X.y, Y.y, Z.y, 0.0f,
		X.z, Y.z, Z.z, 0.0f,
		-X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);

	ViewMatrixInverse = ViewMatrix.Inverted();
}
