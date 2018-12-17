#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "Module.h"
#include <list>
#include <string>
#include "UI_Element.h"
#include "MathGeoLib\MathGeoLib.h"

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
	bool CleanUp();

	void CleanCanvasList(); 

	// Fonts -----------

	Font GetFont(std::string font_name) const; 
	Font* LoadNewFont(std::string font_path, int size);
	void DeleteFont(std::string name); 

	// -----------------

	// Utility ---------

	void AddCanvas(GameObject* canvas_go);
	void DeleteCanvas(GameObject* go);

	void DrawSceneUI(GameObject* camera); 
	void AddaptCanvasToScreen();					// Will addapt all the canvas of the scene (unless they are in world space) to game screen size
	GameObject* GetLastCanvas() const;						// Will return the UI_Canvas of the last GameObject in the list

	AABB GetRenderBox() const;

	// ----------------

private:
	std::list<GameObject*> go_with_canvas; 
	AABB ui_render_box;

	FT_Library ft_library; 
	std::list<Font*> fonts_face_list; 
};

#endif

