#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"

ModuleImGui::ModuleImGui(bool start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Start()
{
	name = "ImGui";
	ImGui_ImplSdlGL2_Init(App->window->window);
	SetDefaultStyle(); 

	return true;
}

void ModuleImGui::SetDefaultStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.05f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.55f, 0.25f, 0.25f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.52f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.75f, 0.71f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.24f, 0.52f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.52f, 0.58f, 0.06f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.57f, 0.55f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.82f, 0.82f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.62f, 0.43f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.89f, 0.74f, 0.00f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.65f, 0.62f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.96f, 0.31f, 0.04f, 0.45f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.33f, 0.59f, 0.17f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.74f, 0.68f, 0.13f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.74f, 0.67f, 0.13f, 0.80f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.94f, 1.00f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.90f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.96f, 0.93f, 0.07f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.ItemSpacing.y = 6;
	style.GrabRounding = 3;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	DrawTopBar();
	App->renderer3D->SetUIPrintSettings();
	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{

	ImGui_ImplSdlGL2_Shutdown();

	return true;
}

void ModuleImGui::DrawTopBar()
{
	ImGui::Begin("Yo no soy don omar, pero soy bandolero"); 
	ImGui::End(); 
}
