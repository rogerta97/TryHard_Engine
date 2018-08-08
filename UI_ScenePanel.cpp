#include "UI_ScenePanel.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui_dock.h"

UI_ScenePanel::UI_ScenePanel()
{
}

UI_ScenePanel::~UI_ScenePanel()
{
}

bool UI_ScenePanel::Start()
{
	show = true;
	return true;
}

bool UI_ScenePanel::Update()
{
	if (ImGui::BeginDock("Scene"))
	{
		//ImGui::SetNextWindowSize(ImVec2(App->window->screen_surface->w, App->window->screen_surface->h), ImGuiSetCond_FirstUseEver);

		//
		//	//// Get the current cursor position (where your window is)
		//	//ImVec2 pos = ImGui::GetCursorScreenPos();

		//	// A boolean to allow me to stop the game rendering
		//	if (runApp) {
		//		glViewport(0, 0, W, H);
		//		// Render the scene into an FBO
		//		game->render(time);
		//		// Restore previous viewport
		//		glViewport(0, 0, w, h);
		//	}
		//	// Get the texture associated to the FBO
		//	auto tex = game->getRendered();

		//	// Ask ImGui to draw it as an image:
		//	// Under OpenGL the ImGUI image type is GLuint
		//	// So make sure to use "(void *)tex" but not "&tex"
		//	ImGui::GetWindowDrawList()->AddImage((void *)tex, ImVec2(ImGui::GetItemRectMin().x + pos.x, ImGui::GetItemRectMin().y + pos.y),
		//		ImVec2(pos.x + h / 2, pos.y + w / 2), ImVec2(0, 1), ImVec2(1, 0));
	
	}
	ImGui::EndDock();

	return true;
}

