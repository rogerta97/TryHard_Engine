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
	CLIP_CENTER,
	CLIP_NONE, 
};


enum horizontalTextOverflow
{
	HORIZONTAL_OVERFLOW,
	HORIZONTAL_WRAP,
};

enum verticalTextOverflow
{
	VERTICAL_OVERFLOW,
	VERTICAL_TRUNCATE,
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
	void UpdateContainerPlane();
	void RenderContainerPlane(); 
	float3 GetContainerPlaneCenter();

	// Wrapping --------------------

	horizontalTextOverflow GetHorizontalOverflow() const;
	void SetHorizontalOverflow(const horizontalTextOverflow new_ov);

	verticalTextOverflow GetVerticalOverflow() const;
	void SetVerticalOverflow(const verticalTextOverflow new_ov);

	float3 container_plane_vertices[4];
	int line_spacing = 0;

private:

	horizontalTextOverflow horizontal_overflow = HORIZONTAL_OVERFLOW;	// Horizontal text wrapping
	verticalTextOverflow vertical_overflow = VERTICAL_OVERFLOW;			// vertical text wrapping	
	
	UI_Label * label;
	float3 draw_color; 
	ClipTextType clipping = ClipTextType::CLIP_BOTTOMLEFT; 
};

