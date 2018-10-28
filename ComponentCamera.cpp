#include "ComponentCamera.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "DebugDraw.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"


ComponentCamera::ComponentCamera(GameObject* parent)
{
	SetGameObject(parent);
	component_type = CMP_CAMERA; 

	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM); 

	Position = trans->transform.position;
	Reference = float3(0.0f, 0.0f, 0.0f);

	camera = new Camera(); 
	camera->InitCamera(); 
	draw_frustum = true; 

	show_preview_panel = true;

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

	UpdateFrustumPositionAndRotation();

	if (draw_frustum)
		DrawFrustum(); 



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
	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM); 

	if (trans != nullptr)
	{
		Reference = Spot;
		
		Z = (Position - Reference).Normalized();
		X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
		Y = Cross(Z, X);

		CalculateViewMatrix();
	}
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

float3 ComponentCamera::GetCamPointFromDistance(float3 center, float distance) const
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
		ComponentMesh* cmp_mesh = (ComponentMesh*)App->scene->GetSelectedGameObject()->GetComponent(CMP_MESH);

		if (selected_go->GetNumChilds() == 0)
		{
			float distance = cmp_mesh->bounding_box.Diagonal().Length() + 1;

			float3 center = cmp_mesh->bounding_box.CenterPoint();

			float3 dst_point = GetCamPointFromDistance(cmp_mesh->bounding_box.CenterPoint(), distance);
			
			interpolation.line.a = dst_point;
			interpolation.line.b = float3({ Position.x, Position.y, Position.z });

			interpolation.dst_vec = -(Position - center).Normalized();
			interpolation.source_vec = -Z;

			interpolation.center = cmp_mesh->bounding_box.CenterPoint();
			
		}
		else //if not find the middle point between the object and look at it. 
		{
			ComponentMesh* parent_mesh = (ComponentMesh*)selected_go->GetComponent(CMP_MESH);

			float3 min, max, position;
			float distance; 

			min = max = position = { 0,0,0 };

			selected_go->GetEnclosedAABB(min, max);
			selected_go->SetCenterCamDataRecursive(position, distance);

			AABB new_bb; 
			new_bb.minPoint = min; 
			new_bb.maxPoint = max; 
			distance = new_bb.Diagonal().Length() + 1; 

			float3 center = position / App->scene->GetGameObjectsAmmount();

			float3 dst_point = GetCamPointFromDistance(center, distance);

			interpolation.center = center;
			
			interpolation.line.a = dst_point;
			interpolation.line.b = Position;
		
			interpolation.dst_vec = -(Position - center).Normalized();
			interpolation.source_vec = -Z;
		}
	}
}

bool ComponentCamera::InterpolateCamera(float time)
{
	float3 look_point = { 0,0,0 };

	float3 center = { interpolation.center.x, interpolation.center.y, interpolation.center.z };

	float3 src_vector = { interpolation.source_vec.x,  interpolation.source_vec.y , interpolation.source_vec.z };
	float3 dst_vector = { interpolation.dst_vec.x,  interpolation.dst_vec.y , interpolation.dst_vec.z };
	float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, 1);

	bool end = false;

	if (Reference.x == center.x && Reference.y == center.y && Reference.z == center.z) {
		end = true;
		if (!AreFloat3Same(src_vector, dst_vector, 0.001f))
			end = false;
	}

	if (end) {
		interpolation.interpolate = false;
		return true;
	}

	if (interpolation.interpolation_timer.Read() <= time)
	{
		float percentage = (float)interpolation.interpolation_timer.Read() / time;

		//Interpolate Position
		Position = { interpolation.line.GetPoint(1 - percentage).x, interpolation.line.GetPoint(1 - percentage).y, interpolation.line.GetPoint(1 - percentage).z };

		//Interpolate Rotation
		float3 src_vector = { interpolation.source_vec.x,  interpolation.source_vec.y , interpolation.source_vec.z };
		float3 dst_vector = { interpolation.dst_vec.x,  interpolation.dst_vec.y , interpolation.dst_vec.z };

		float3 curr_rot = Quat::SlerpVector(src_vector, dst_vector, percentage);

		look_point = { curr_rot.x, curr_rot.y, curr_rot.z };

		LookAt(Position + look_point);

		return false;
	}
	else
	{
		interpolation.interpolate = false;
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

void ComponentCamera::SetViewportTextureSize(uint x, uint y)
{
	viewport_texture->SetWidth(x);
	viewport_texture->SetHeight(y);
}

void ComponentCamera::UpdateFrustumPositionAndRotation()
{
	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

	camera->frustum.pos = trans->GetPosition();

	float3 eul = trans->GetRotationEuler();

	camera->frustum.front = App->camera->Rotate(trans->transform.Z, eul.x * DEGTORAD, trans->transform.X);
	trans->transform.Z = camera->frustum.front; 

	camera->frustum.up = App->camera->Rotate(trans->transform.Y, eul.x * DEGTORAD, trans->transform.X);
	trans->transform.Y = camera->frustum.up;

	camera->frustum.front = App->camera->Rotate(camera->frustum.front, eul.y * DEGTORAD, trans->transform.Y);
	camera->frustum.up = App->camera->Rotate(camera->frustum.up, eul.y * DEGTORAD, trans->transform.Y);

	camera->frustum.front = App->camera->Rotate(camera->frustum.front, eul.z * DEGTORAD, trans->transform.Z);
	camera->frustum.up = App->camera->Rotate(camera->frustum.up, eul.z * DEGTORAD, trans->transform.Z);

	trans->transform.X = trans->transform.Z.Cross(trans->transform.Y).Normalized();
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

Frustum ComponentCamera::GetFrustum() const
{
	return camera->frustum;
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

void ComponentCamera::DrawFrustum()
{
	App->renderer3D->UseDebugRenderSettings();
	float3 vertices[8]; 
	camera->frustum.GetCornerPoints(vertices);
	DebugDrawBox(vertices, Color(0.0f, 1.0f, 1.0f));

	App->renderer3D->UseCurrentRenderSettings();
}

void ComponentCamera::PrintPreview()
{
	if (ImGui::Begin("test", &show_preview_panel))
	{
		//Render the texture
		glEnable(GL_TEXTURE_2D);
		if (GetViewportTexture() != nullptr)
		{
			GetViewportTexture()->Bind();

			ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glLoadIdentity();

			glMatrixMode(GL_MODELVIEW);

			glLoadMatrixf(&trans->GetViewMatrix()[0][0]);

			App->camera->GetEditorCamera()->camera->projection_changed = true;

			glMatrixMode(GL_PROJECTION);
			Frustum camerafrustum = camera->frustum;
			float4x4 ProjectionMatrix = camerafrustum.ProjectionMatrix();

			glLoadMatrixf(&ProjectionMatrix[0][0]);


			ImVec2 region_size = ImGui::GetContentRegionAvail();
			const float region_ratio = region_size.y / region_size.x;

			//Camera* camera = App->camera->GetEditorCamera()->camera;

			camera->SetAspectRatio(1 / region_ratio);

			ImGui::Image((void*)GetViewportTexture()->GetTextureID(), region_size, ImVec2(0, 1), ImVec2(1, 0));

			//App->camera->GetViewportTexture()->Unbind();
		}
		glDisable(GL_TEXTURE_2D);

	}
	ImGui::End();

	App->camera->GetEditorCamera()->GetViewportTexture()->Render();
	App->camera->GetEditorCamera()->GetViewportTexture()->Unbind();
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(
		X.x, Y.x, Z.x, 0.0f,
		X.y, Y.y, Z.y, 0.0f,
		X.z, Y.z, Z.z, 0.0f,
		-X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);

	ViewMatrixInverse = ViewMatrix.Inverted();
}
