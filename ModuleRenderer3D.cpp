#include "Globals.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "ModuleCamera3D.h"
#include "OpenGL.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ComponentCamera.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled)
{
	name = "Renderer";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* config)
{
	CONSOLE_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if(context == NULL)
	{
		CONSOLE_ERROR("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	glewInit(); 

	if(ret == true)
	{
		ImGui::CreateContext();


		//Use Vsync
		if (App->GetVsync().is_active)
		{
			if (App->GetLastSecFramerate() < 60 && App->GetLastSecFramerate() > 30)
				App->GetVsync().SetLevel(1);
			else if (App->GetLastSecFramerate() < 30 && App->GetLastSecFramerate() > 16)
				App->GetVsync().SetLevel(2);
		} 
		else
		{
			App->GetVsync().SetLevel(0); 
		}

		if (VSYNC && SDL_GL_SetSwapInterval(0) < 0)
			CONSOLE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_ERROR("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLE_ERROR("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.4f, 0.4f, 0.4f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0 ].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//This should be load from JSON files
		render_settings.depth_test = true; 
		render_settings.cull_face = true;
		render_settings.wireframe = false;
		render_settings.color_material = true;

		lights[0].Active(true);
		glEnable(GL_LIGHTING);

		UseCurrentRenderSettings();
	}

	// Projection matrix for
	OnResize(App->window->width, App->window->height);

	init_time = performance_timer.Read();
	return ret; 
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetEditorCamera()->GetViewMatrix());

	glMatrixMode(GL_PROJECTION);
	Frustum camerafrustum = App->camera->GetEditorCamera()->camera->frustum;
	ProjectionMatrix = camerafrustum.ProjectionMatrix();

	glLoadMatrixf(&ProjectionMatrix[0][0]);

	//mat4x4 ProjectionMatrix = perspective(60.0f, (float)App->window->GetWidth() / (float)App->window->GetHeight(), 0.125f, 512.0f);
	//glLoadMatrixf(App->camera->GetEditorCamera()->GetViewMatrix());
	//glLoadMatrixf(&ProjectionMatrix);


	//for (auto it = rendering_cameras.begin(); it != rendering_cameras.end(); it++)
	//{
	//	if ((*it)->GetProjection() == PROJ_ORTH)
	//	{
	//		mat4x4 ort = ortho(1, 1, 1, 1, 0.2, 20);
	//		glLoadMatrixf(ort.M);
	//	}
	//	else if ((*it)->GetProjection() == PROJ_PERSP)
	//	{
			//mat4x4 ProjectionMatrix = perspective(60.0f, (float)App->window->GetWidth() / (float)App->window->GetHeight(), 0.125f, 512.0f);
			////glLoadMatrixf(&ProjectionMatrix);
			//glLoadMatrixf(App->camera->GetEditorCamera()->GetViewMatrix());
	//	}
	//}

	lights[0].SetPos(App->camera->GetEditorCamera()->Position.x, App->camera->GetEditorCamera()->Position.y, App->camera->GetEditorCamera()->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	App->camera->GetEditorCamera()->GetViewportTexture()->Bind();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	//glViewport(0, 0, width, height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//if (App->camera->GetEditorCamera()) {

	//	ProjectionMatrix = App->camera->GetEditorCamera()->camera->frustum.ProjectionMatrix();

	//	glLoadMatrixf(&ProjectionMatrix[0][0]);
	//}

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void ModuleRenderer3D::UseUIRenderSettings()
{
	GLfloat LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

void ModuleRenderer3D::UseDebugRenderSettings()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(DEFAULT_WIREFRAME_COLOR);
}

char* ModuleRenderer3D::GetGraphicsModel() const
{
	return (char*)glGetString(GL_RENDERER);
}

char * ModuleRenderer3D::GetGraphicsVendor() const
{
	return (char*)glGetString(GL_VENDOR);;
}

void ModuleRenderer3D::UseCurrentRenderSettings()
{
	if (render_settings.depth_test)
		glEnable(GL_DEPTH_TEST);
	else 
		glDisable(GL_DEPTH_TEST);

	if (render_settings.cull_face)
		glEnable(GL_CULL_FACE);
	else  
		glDisable(GL_CULL_FACE);

	if (render_settings.color_material)
		glEnable(GL_COLOR_MATERIAL);
	else 
		glDisable(GL_COLOR_MATERIAL);

	if (render_settings.texture)
		glEnable(GL_TEXTURE_2D);
	else 
		glDisable(GL_TEXTURE_2D);

	if (render_settings.light)
		glEnable(GL_LIGHTING);
	else  
		glDisable(GL_LIGHTING);

	if (render_settings.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(DEFAULT_WIREFRAME_COLOR);
	}		
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(DEFAULT_GEOMETRY_COLOR);
	}
		
}

RenderSettings ModuleRenderer3D::GetDefaultRenderSettings() const
{
	return render_settings;
}

void ModuleRenderer3D::AddRenderCamera(ComponentCamera * cam)
{
	rendering_cameras.push_back(cam); 
}

//void ModuleRenderer3D::SetRenderCamera(ComponentCamera * cam)
//{
//	if(cam != nullptr)
//		rendering_camera = cam; 
//}
//
//ComponentCamera * ModuleRenderer3D::GetRenderCamera() const
//{
//	return rendering_camera;
//}

void ModuleRenderer3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		//Draw info
		bool go = false; 

		ImGui::Checkbox("Depth Test", &render_settings.depth_test); 
		
		ImGui::Checkbox("Cull Face", &render_settings.cull_face);

		ImGui::Checkbox("Lightning", &render_settings.light);
	
		ImGui::Checkbox("Wireframe", &render_settings.wireframe);

		ImGui::Checkbox("Color Material", &render_settings.color_material);

		ImGui::Checkbox("2D Texture", &render_settings.texture);

		ImGui::Checkbox("Wireframe Selected", &render_settings.wireframe_selected);
	
	}
}
