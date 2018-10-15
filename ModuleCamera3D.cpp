#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "UI_InspectorPanel.h"

#include "ComponentMesh.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	//name = "Camera";

	//CalculateViewMatrix();

	//X = vec3(1.0f, 0.0f, 0.0f);
	//Y = vec3(0.0f, 1.0f, 0.0f);
	//Z = vec3(0.0f, 0.0f, 1.0f);

	//Position = vec3(3.0f, 3.0f, 8.0f);
	//Reference = vec3(0.0f, 0.0f, 0.0f);

	//orbit = true;
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
	skybox->InitSkyBox("");

	//Create Editor Camera
	ecam_go = new GameObject("EditorCamera"); 

	ComponentCamera* cam = (ComponentCamera*)ecam_go->AddComponent(CMP_CAMERA);
	cam->SetProjection(PROJ_PERSP); 
	cam->SetAspect(ASP_EDITOR);

	App->renderer3D->SetRenderCamera(cam); 

	start_time = performance_timer.Read();
	return ret;
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

		ImGui::Spacing();
		ImGui::Text("Editor Camera:");
		ImGui::Separator();
		ImGui::Spacing(); 

		float show_pos[3] = { editor_camera->Position.x,  editor_camera->Position.y,  editor_camera->Position.z };
	 
		ImGui::InputFloat3("Position", show_pos, 2);
		//ImGui::InputFloat3("Rotation", show_rot, 2);

		float tmp_speed = editor_camera->GetSpeed();
		ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
		editor_camera->SetSpeed(tmp_speed);

		float tmp_sensitivity = editor_camera->GetMouseSensitivity();
		ImGui::SliderFloat("Rotation Speed", &tmp_sensitivity, 0.01f, 1.0f, "%.2f");
		ImGui::SameLine(); App->imgui->ShowHelpMarker("Hold the right mouse button\n" "and drag to rotate the camera.\n");
		editor_camera->SetMouseSensitivity(tmp_sensitivity);

		ImGui::SliderFloat("Zoom Spped", &editor_camera->wheel_zoom_speed, 0.01f, 10.0f, "%.2f");

		ImGui::SliderInt("Interpolation Speed", (int*)&editor_camera->interpolation.interpolation_ms, 50, 2000);
	
		App->imgui->inspector_panel->PrintCameraProperties(App->camera->GetEditorCamera());

		ImGui::Spacing();

		ImGui::Text("Main Camera:");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("GameObject Bounded:"); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "*NONE*"); ImGui::SameLine(); 

		if(ImGui::SmallButton("+##Camera"))
		{

		}

		ImGui::Spacing();
	}
}

ComponentCamera * ModuleCamera3D::GetEditorCamera()
{
	ComponentCamera* cam = (ComponentCamera * )ecam_go->GetComponent(CMP_CAMERA);

	if(cam) 
		return cam;
	else 
		return nullptr; 	
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
	if (ecam_go != nullptr && cam != nullptr)
	{
		//Editor Camera Movement
		vec3 point_look = { 0, 0, 0 };
		int scale_value = 3;
		bool moved = false;
		cam->speed_multiplier = 1;

		//Camera WASD & ER input
		vec3 increment = { 0.0f ,0.0f ,0.0f };

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

		if (App->input->GetMouseWheel() < 0) {
			increment -= cam->Z * -cam->GetSpeed()*App->GetDt() *cam->speed_multiplier * cam->wheel_zoom_speed * 30;
			moved = true;
		}

		if (App->input->GetMouseWheel() > 0) {
			increment += cam->Z * -cam->GetSpeed()*App->GetDt() * cam->speed_multiplier * cam->wheel_zoom_speed * 30;
			moved = true;
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

					cam->X = rotate(cam->X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					cam->Y = rotate(cam->Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					cam->Z = rotate(cam->Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}



				if (dy != 0)
				{
					float DeltaY = (float)dy * cam->mouse_sensitivity;

					cam->Y = rotate(cam->Y, DeltaY, cam->X);
					cam->Z = rotate(cam->Z, DeltaY, cam->X);

					if (cam->Y.y < 0.0f)
					{
						cam->Z = vec3(0.0f, cam->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						cam->Y = cross(cam->Z, cam->X);
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					cam->Position = cam->Reference + cam->Z * length(cam->Position);
			}
		}

		cam->CalculateViewMatrix();

		//For now we will update editor camera directly 
		
		ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA); 
		cam->Update(); 
	}

	
	return UPDATE_CONTINUE;
}
