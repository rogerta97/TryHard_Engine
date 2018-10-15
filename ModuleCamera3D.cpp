#include "Globals.h"
#include "TextureMSAA.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

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
	ecam_go = new GameObject(); 
	ecam_go->AddComponent(ecam_go->CreateComponent(CMP_CAMERA));

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

		SEPARATE_WITH_SPACE

		float show_pos[3] = { editor_camera->Position.x,  editor_camera->Position.y,  editor_camera->Position.z };
	 
		ImGui::InputFloat3("Position", show_pos, 2);
		//ImGui::InputFloat3("Rotation", show_rot, 2);

		SEPARATE_WITH_SPACE

		float tmp_speed = editor_camera->GetSpeed();
		ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
		editor_camera->SetSpeed(tmp_speed);
		float tmp_sensitivity = editor_camera->GetMouseSensitivity();
		ImGui::SliderFloat("Rotation Speed", &tmp_sensitivity, 0.01f, 1.0f, "%.2f");
		ImGui::SameLine(); App->imgui->ShowHelpMarker("Hold the right mouse button\n" "and drag to rotate the camera.\n");
		editor_camera->SetMouseSensitivity(tmp_sensitivity);

		ImGui::SliderFloat("Zoom Spped", &editor_camera->wheel_zoom_speed, 0.01f, 10.0f, "%.2f");

		ImGui::SliderInt("Interpolation Speed", (int*)&editor_camera->cam_interpolation.interpolation_ms, 50, 2000);

		SEPARATE_WITH_SPACE
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

	//For now we will update editor camera directly 
	if (ecam_go != nullptr && ecam_go->GetComponent(CMP_CAMERA) != nullptr)
	{
		ComponentCamera* cam = (ComponentCamera*)ecam_go->GetComponent(CMP_CAMERA); 
		cam->Update(); 
	}

	
	return UPDATE_CONTINUE;
}
