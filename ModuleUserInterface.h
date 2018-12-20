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
	void AddaptCanvasToScreen();					// Will addapt all the canvas of the scene (unless they are in world space) to game screen size
	GameObject* GetLastCanvas() const;						// Will return the UI_Canvas of the last GameObject in the list

	AABB GetRenderBox() const;

	// ----------------

private:

	std::list<char> buttons_pressed; 
	std::list<GameObject*> go_with_canvas; 
	AABB ui_render_box;

	FT_Library ft_library; 
	std::list<Font*> fonts_face_list; 
};

