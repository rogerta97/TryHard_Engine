#pragma once

#include "Module.h"
#include <list>
#include <string>
#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#pragma comment(lib, "FreeType/win32/freetype.lib") 

class GameObject;
class Font; 

class ModuleUserInterface : public Module
{
public:
	ModuleUserInterface();
	~ModuleUserInterface();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CleanCanvasList(); 

	void EnableUI(bool new_value); 

	// Fonts -----------

	Font GetFont(std::string font_name) const; 
	Font* LoadNewFont(std::string font_path, int size);
	void LoadAllFonts(); 
	void DeleteFont(std::string name); 

	// -----------------

	void RecieveEvent(const Event & curr_event);
	void SendInput(SDL_Event* e);							//Will get the event and store the clicked buttons to a list
	std::list<char>& GetInputLastFrame();					// Get the buttons pressed las frame

	// Utility ---------

	void AddCanvas(GameObject* canvas_go);
	void DeleteCanvas(GameObject* go);

	void DrawSceneUI(GameObject* camera); 
	void AddaptCanvasToScreen();							// Will addapt all the canvas of the scene (unless they are in world space) to game screen size
	GameObject* GetLastCanvas() const;						// Will return the UI_Canvas of the last GameObject in the list

	AABB GetRenderBox() const;

	float3 GetMousePos() const;
	void SetMousePos(const float3& new_pos);

	void InterpolateAlpha();						// Will interpolate the alpha of the UI elements 

	// ----------------

	void SetInterpolation(bool value, float time = 0.0f);
	bool IsInterpolating(); 
	bool HasInterpolationEnded();

private:

	bool interpolating = false;
	bool finished_interpolation = false;
	float interpolate_in = 0.0f;
	Timer interpolation_timer = Timer();

	std::list<char> buttons_pressed = std::list<char>();
	std::list<GameObject*> go_with_canvas = std::list<GameObject*>();
	std::vector<GameObject*> last_intersected_elements = std::vector<GameObject*>();
	AABB ui_render_box = AABB();

	FT_Library ft_library = FT_Library();
	std::list<Font*> fonts_face_list = std::list<Font*>();

	float3 mouse_game_pos; 
};