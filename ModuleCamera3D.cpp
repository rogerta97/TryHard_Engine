#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"

#include "DebugDraw.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	name = "Camera";
	ecam_go = nullptr;
	frustum_culling = false;
	is_ghost_camera = false;
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
	skybox->InitSkyBox("", 25.0f);

	//Create Editor Camera
	ecam_go = new GameObject("EditorCamera");

	ComponentTransform* ecam_trans = (ComponentTransform*)ecam_go->GetComponent(CMP_TRANSFORM);
	ecam_trans->SetPosition({ 10, 4, 10 });

	ComponentCamera* cam = (ComponentCamera*)ecam_go->AddComponent(CMP_CAMERA);
	cam->camera->aspect = ASP_EDITOR;
	cam->camera->frustum.farPlaneDistance = 500;
	cam->SetEditorCamera();
	cam->draw_frustum = false;
	cam->camera->frustum.pos = { 10, 4, 10 };
	cam->LookAt({ 0,0,0 });
	cam->is_editor = true; 

	App->renderer3D->AddRenderCamera(cam);
	skybox->AttachTo(ecam_go); 

	//Create Game Camera
	gcam_go = nullptr; 

	start_time = performance_timer.Read();

	mouse_picking.picking.a = { 0,0,0 };
	mouse_picking.picking.b = { 0,0,0 };

	return ret;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA);
	cam->Update();
	ManageMovement();

	// Mouse Picking ----------------
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->imgui->is_mouse_in_scene)
	{
		ImVec2 mouse_pos_norm = App->imgui->scene_panel->GetMousePosInDockNormalized();

		if (mouse_pos_norm.x > -1 && mouse_pos_norm.x < 1)
			if (mouse_pos_norm.y > -1 && mouse_pos_norm.y < 1)
				mouse_picking.picking = cam->GetFrustum()->UnProjectLineSegment(mouse_pos_norm.x, mouse_pos_norm.y);

		if (mouse_picking.picking.Length() != 0)
			App->scene->TestLineAgainstGOs(mouse_picking.picking);

	}


	App->renderer3D->UseDebugRenderSettings();

	DebugDraw(mouse_picking.picking, Color(1.0f, 0.0f, 1.0f), false, float4x4::identity, 3.0f);

	App->renderer3D->UseCurrentRenderSettings();

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

		ImGui::Checkbox("Frustum Culling", &frustum_culling);

		ImGui::Checkbox("Ghost Camera", &is_ghost_camera);

		if(ImGui::TreeNode("Editor Camera Settings"))
		{	
			App->imgui->inspector_panel->PrintCameraProperties(App->camera->GetEditorCamera());

			ImGui::TreePop(); 
		}
		
		SEPARATE_WITH_SPACE

		ImGui::Text("Main Camera:");
	
		ImGui::SameLine();

		if(gcam_go == nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "NONE");
		else
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", gcam_go->GetName().c_str());

		ImGui::SameLine();

		if (ImGui::SmallButton("+##Camera"))
		{
			ImGui::OpenPopup("select_camera");
		}

		ImGui::SameLine();

		if (ImGui::SmallButton("X##Camera"))
		{
			gcam_go = nullptr; 
			App->renderer3D->rendering_cameras.pop_back();
		}

		if(ImGui::BeginPopup("select_camera"))
		{
			ImGui::Text("GO with Camera:");
			ImGui::Separator();
			int i = 0;

			std::list<GameObject*> go_with_cam = App->scene->GetAllGameObjectsWith(CMP_CAMERA);

			for (auto it = go_with_cam.begin(); it != go_with_cam.end(); it++) 
			{			
				if (ImGui::Selectable((*it)->GetName().c_str()))
				{
					gcam_go = (*it); 
					App->renderer3D->rendering_cameras.push_back((ComponentCamera*)gcam_go->GetComponent(CMP_CAMERA));
					break;
				}
				
			}
			ImGui::EndPopup();
		}
		ImGui::Spacing();

		if (gcam_go != nullptr)
		{
			if (ImGui::TreeNode("Main Camera Settings"))
			{
				ComponentCamera* cam = (ComponentCamera*)gcam_go->GetComponent(CMP_CAMERA);
				App->imgui->inspector_panel->PrintCameraProperties(cam);

				ImGui::TreePop();
			}
		}
	
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

ComponentCamera * ModuleCamera3D::GetGameCamera()
{
	if (!gcam_go)
		return nullptr;
	ComponentCamera* cam = (ComponentCamera *)gcam_go->GetComponent(CMP_CAMERA);

	if (cam)
		return cam;
	else
		return nullptr;
}

void ModuleCamera3D::SetGameCamera(GameObject * new_cam)
{
	gcam_go = new_cam; 
}

bool ModuleCamera3D::IsGhostCamera() const
{
	return is_ghost_camera;
}

void ModuleCamera3D::SetGhostCamera(bool value)
{
	is_ghost_camera = value;
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

		// Mouse motion ----------------
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


		if (!is_ghost_camera) {
			cam->GetFrustum()->SetWorldMatrix(mat->Float3x4Part());
			cam->GetFrustum()->Translate(cam->Position);
		}
	}
}

