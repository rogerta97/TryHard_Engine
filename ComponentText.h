#pragma once
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class UI_Label; 

enum ClipTextType
{
	CLIP_TOPLEFT,
	CLIP_BOTTOMLEFT,
	CLIP_TOPRIGHT,
	CLIP_BOTTOMRIGHT,
	CLIP_NONE, 
};

class ComponentText : public Component
{
public:
	ComponentText(GameObject* parent);
	~ComponentText();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	// Utility ---------------------

	UI_Label* GetLabel() const;

	ClipTextType GetClipping() const; 
	void SetClipping(const ClipTextType new_clip); 
	void TranslateEnclosedPlane(float3 increment);

private:

	float3 container_plane_vertices[4];
	UI_Label * label;
	float3 draw_color; 
	ClipTextType clipping = ClipTextType::CLIP_BOTTOMLEFT; 
};

