#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "UI_InspectorPanel.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	name = "Camera";
	ecam_go = nullptr;
	frustum_culling = false;
}

ModuleCamera3D::~ModuleCamera3D()
{

}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	skybox = new SkyBox();
	skybox->InitSkyBox("", 300.0f);

	//Create Editor Camera
	ecam_go = new GameObject("EditorCamera");
	ComponentTransform* cam_trans = (ComponentTransform*)ecam_go->AddComponent(CMP_TRANSFORM);

	cam_trans->SetPosition({ 4, 4, 8 });

	ComponentCamera* cam = (ComponentCamera*)ecam_go->AddComponent(CMP_CAMERA);
	cam->camera->aspect = ASP_EDITOR;
	cam->camera->frustum.farPlaneDistance = 500;
	cam->SetEditorCamera();
	cam->draw_frustum = false;
	cam->camera->frustum.pos = { 4,4,8 };

	App->renderer3D->AddRenderCamera(cam);

	start_time = performance_timer.Read();
	return ret;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	//SkyBox
	if (skybox != nullptr)
	{
		skybox->Draw();
	}

	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);
	cam->Update();
	//ManageMovementFromTrans(cam);
	//ManageMovement();
	MoveRotateECamFrustum(dt);

	cam->CalculateViewMatrix();

	if (!ecam_go || !cam)
		return UPDATE_ERROR;


	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	if (skybox != nullptr)
	{
		skybox->CleanUpSkyBox();
	}

	return true;
}

void ModuleCamera3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		//Get the editor camera 
		ComponentCamera* editor_camera = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);
		ComponentTransform* ecam_trans = (ComponentTransform*)ecam_go->GetComponent(CMP_TRANSFORM);

		ImGui::Spacing();
		ImGui::Text("Editor Camera:");
		ImGui::Spacing();

		if (ecam_trans)
		{
			ImGui::Spacing();

			ImGui::SmallButton("Local"); ImGui::SameLine(); ImGui::SmallButton("Global"); ImGui::SameLine();

			if (ImGui::SmallButton("Reset"))
			{
				ecam_trans->ResetTransform();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float show_pos[3] = { ecam_trans->GetPosition().x, ecam_trans->GetPosition().y, ecam_trans->GetPosition().z };
			float show_rot[3] = { ecam_trans->GetRotationEuler().x, ecam_trans->GetRotationEuler().y, ecam_trans->GetRotationEuler().z };
			float show_scale[3] = { ecam_trans->GetScale().x, ecam_trans->GetScale().y, ecam_trans->GetScale().z };

			if (ImGui::DragFloat3("Position", show_pos, 0.2f))
				ecam_trans->SetPosition({ show_pos[0], show_pos[1], show_pos[2] });

			if (ImGui::DragFloat3("Rotation", show_rot, 0.2f, -180.0f, 180.0f))
			{
				if (ecam_trans->GetRotationEuler().x != show_rot[0])
					ecam_trans->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

				if (ecam_trans->GetRotationEuler().y != show_rot[1])
					ecam_trans->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

				if (ecam_trans->GetRotationEuler().z != show_rot[2])
					ecam_trans->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });
			}

			ImGui::Spacing();
		}


		ImGui::Separator();
		ImGui::Spacing();

		float tmp_speed = editor_camera->GetSpeed();
		ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
		editor_camera->SetSpeed(tmp_speed);

		float tmp_sensitivity = editor_camera->GetMouseSensitivity();
		ImGui::SliderFloat("Rotation Speed", &tmp_sensitivity, 0.01f, 1.0f, "%.2f");
		ImGui::SameLine(); App->imgui->ShowHelpMarker("Hold the right mouse button\n" "and drag to rotate the camera.\n");
		editor_camera->SetMouseSensitivity(tmp_sensitivity);

		ImGui::SliderFloat("Zoom Spped", &editor_camera->wheel_zoom_speed, 0.01f, 10.0f, "%.2f");

		ImGui::SliderInt("Interpolation Speed", (int*)&editor_camera->interpolation.interpolation_ms, 50, 2000);

		SEPARATE_WITH_SPACE

			App->imgui->inspector_panel->PrintCameraProperties(App->camera->GetEditorCamera());

		ImGui::Spacing();

		ImGui::Text("Main Camera:");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("GameObject Bounded:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "*NONE*");

		ImGui::Checkbox("Frustum Culling", &frustum_culling);


		if (ImGui::SmallButton("+##Camera"))
		{

		}

		ImGui::Spacing();
	}
}

ComponentCamera * ModuleCamera3D::GetEditorCamera()
{
	if (!ecam_go)
		return nullptr;
	ComponentCamera* cam = (ComponentCamera *)ecam_go->GetComponent(CMP_CAMERA);

	if (cam)
		return cam;
	else
		return nullptr;
}

float3 ModuleCamera3D::Rotate(const float3 & u, float angle, const float3 & v)
{
	return *(float3*)&(float4x4::RotateAxisAngle(v, angle) * float4(u, 1.0f));
}

GameObject * ModuleCamera3D::GetCameraGO() const
{
	return ecam_go;
}

void ModuleCamera3D::ManageMovementFromTrans(ComponentCamera * cam)
{
	if (ecam_go != nullptr && cam != nullptr)
	{
		//Editor Camera Movement
		float3 point_look = { 0, 0, 0 };
		int scale_value = 3;
		bool moved = false;
		cam->speed_multiplier = 1;

		//Camera WASD & ER input
		float3 increment = { 0.0f ,0.0f ,0.0f };

		ComponentTransform* camera_trans = (ComponentTransform*)ecam_go->GetComponent(CMP_TRANSFORM);

		if (camera_trans == nullptr)
			return;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
			cam->speed_multiplier = 2;

		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			increment = camera_trans->transform.Z * cam->GetSpeed()*App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_S))
		{
			increment = camera_trans->transform.Z * -cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_A))
		{
			increment = camera_trans->transform.X * -cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_D))
		{
			increment = camera_trans->transform.X * cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			increment = camera_trans->transform.Y * cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			increment = camera_trans->transform.Y * -cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			camera_trans->SetPosition(camera_trans->GetPosition() + increment);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->scene->GetSelectedGameObject() != nullptr)
			{
				cam->interpolation.interpolate = true;
				cam->interpolation.interpolation_timer.Start();
				cam->FillInterpolationSegmentAndRot();
			}
		}

		if (App->imgui->is_mouse_in_scene) {

			if (App->input->GetMouseWheel() < 0) {
				float new_fov = cam->camera->GetFov() - (1 * cam->wheel_zoom_speed) * DEGTORAD;
				if (new_fov > 0.01f)
					cam->camera->SetFOV(new_fov);
			}

			if (App->input->GetMouseWheel() > 0) {
				float new_fov = cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD;
				if (new_fov < 179.0f * DEGTORAD)
					cam->camera->SetFOV(cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD);
			}
		}


		if (cam->interpolation.interpolate)
		{
			cam->InterpolateCamera(cam->interpolation.interpolation_ms);
		}

		/*	if (moved) {
				cam->Move(increment);
				camera_trans->SetPosition(cam->Position);
			}*/


			//// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dy != 0 || dx != 0) {

				float3 tmp_z = cam->Z;

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					cam->Position -= cam->Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * cam->mouse_sensitivity;

					cam->X = Rotate(cam->X, DeltaX, float3(0.0f, 1.0f, 0.0f));
					cam->Y = Rotate(cam->Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
					cam->Z = Rotate(cam->Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
				}



				if (dy != 0)
				{
					float DeltaY = (float)dy * cam->mouse_sensitivity;

					cam->Y = Rotate(cam->Y, DeltaY, cam->X);
					cam->Z = Rotate(cam->Z, DeltaY, cam->X);

					if (cam->Y.y < 0.0f)
					{
						cam->Z = float3(0.0f, cam->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						cam->Y = Cross(cam->Z, cam->X);
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					cam->Position = cam->Reference + cam->Z * Length(cam->Position);
			}
		}

		camera_trans->CalculateViewMatrix();

		//For now we will update editor camera directly 

		ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);
		cam->Update();
	}
}

void ModuleCamera3D::ManageMovement()
{

	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);

	if (ecam_go != nullptr && cam != nullptr)
	{
		//Editor Camera Movement
		float3 point_look = { 0, 0, 0 };
		int scale_value = 3;
		bool moved = false;
		cam->speed_multiplier = 1;

		//Camera WASD & ER input
		float3 increment = { 0.0f ,0.0f ,0.0f };

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
			cam->speed_multiplier = 2;

		if (App->input->GetKey(SDL_SCANCODE_W))
		{
			increment += cam->Z * -cam->GetSpeed()*App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_S))
		{
			increment += cam->Z * cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_A))
		{
			increment += cam->X * -cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_D))
		{
			increment += cam->X * cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_E))
		{
			increment += cam->Y * cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_R))
		{
			increment += cam->Y * -cam->GetSpeed() * App->GetDt() * cam->speed_multiplier;
			moved = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->scene->GetSelectedGameObject() != nullptr)
			{
				cam->interpolation.interpolate = true;
				cam->interpolation.interpolation_timer.Start();
				cam->FillInterpolationSegmentAndRot();
			}
		}

		if (App->imgui->is_mouse_in_scene) {

			if (App->input->GetMouseWheel() < 0) {
				float new_fov = cam->camera->GetFov() - (1 * cam->wheel_zoom_speed) * DEGTORAD;
				if (new_fov > 0.01f)
					cam->camera->SetFOV(new_fov);
			}

			if (App->input->GetMouseWheel() > 0) {
				float new_fov = cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD;
				if (new_fov < 179.0f * DEGTORAD)
					cam->camera->SetFOV(cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD);
			}
		}


		if (cam->interpolation.interpolate)
		{
			cam->InterpolateCamera(cam->interpolation.interpolation_ms);
		}

		if (moved)
			cam->Move(increment);

		//// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dy != 0 || dx != 0) {

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					cam->Position -= cam->Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * cam->mouse_sensitivity;

					cam->X = Rotate(cam->X, DeltaX, float3(0.0f, 1.0f, 0.0f));
					cam->Y = Rotate(cam->Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
					cam->Z = Rotate(cam->Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
				}



				if (dy != 0)
				{
					float DeltaY = (float)dy * cam->mouse_sensitivity;

					cam->Y = Rotate(cam->Y, DeltaY, cam->X);
					cam->Z = Rotate(cam->Z, DeltaY, cam->X);

					if (cam->Y.y < 0.0f)
					{
						cam->Z = float3(0.0f, cam->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						cam->Y = Cross(cam->Z, cam->X);
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					cam->Position = cam->Reference + cam->Z * Length(cam->Position);
			}
		}

		cam->CalculateViewMatrix();

		ComponentTransform* camera_trans = (ComponentTransform*)ecam_go->GetComponent(CMP_TRANSFORM);

		float4x4* mat = (float4x4*)cam->GetViewMatrix();

		camera_trans->SetViewMatrix(*mat);

		//For now we will update editor camera directly 
	}
}

void ModuleCamera3D::MoveRotateECamFrustum(float dt)
{
	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);

	MoveFrustum(dt);

	

	//Mouse motion
	float motion_x = App->input->GetMouseXMotion();
	float motion_y = App->input->GetMouseYMotion();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0))
	{
		float dx = (float)-motion_x * dt;
		float dy = (float)-motion_y * dt;

		//if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		//	Orbit(dx, dy);
		//else
			RotateFrustum(dx, dy);
	}


	//Wheel changeFov
	if (App->imgui->is_mouse_in_scene) {

		if (App->input->GetMouseWheel() < 0) {
			float new_fov = cam->camera->GetFov() - (1 * cam->wheel_zoom_speed) * DEGTORAD;
			if (new_fov > 0.01f)
				cam->camera->SetFOV(new_fov);
		}

		if (App->input->GetMouseWheel() > 0) {
			float new_fov = cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD;
			if (new_fov < 179.0f * DEGTORAD)
				cam->camera->SetFOV(cam->camera->GetFov() + (1 * cam->wheel_zoom_speed) * DEGTORAD);
		}
	}
}

void ModuleCamera3D::RotateFrustum(float dx, float dy)
{

	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);

	// x motion make the camera rotate in Y absolute axis (0,1,0) (not local)
	if (dx != 0.f)
	{
		Quat q = Quat::RotateY(dx);
		cam->camera->frustum.front = q.Mul(cam->camera->frustum.front).Normalized();
		// would not need this is we were rotating in the local Y, but that is too disorienting
		cam->camera->frustum.up = q.Mul(cam->camera->frustum.up).Normalized();
	}

	// y motion makes the camera rotate in X local axis, with tops
	if (dy != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(cam->camera->frustum.WorldRight(), dy);

		float3 new_up = q.Mul(cam->camera->frustum.up).Normalized();

		if (new_up.y > 0.0f)
		{
			cam->camera->frustum.up = new_up;
			cam->camera->frustum.front = q.Mul(cam->camera->frustum.front).Normalized();
		}
	}
}

void ModuleCamera3D::MoveFrustum(float dt)
{

	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);

	Frustum* frustum = &cam->camera->frustum;

	float adjusted_speed = cam->speed_multiplier;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) adjusted_speed *= 5.0f;
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) adjusted_speed *= 0.5f;

	float3 right(frustum->WorldRight());
	float3 forward(frustum->front);

	float3 movement(float3::zero);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	frustum->Translate(movement * (adjusted_speed * dt));
}
