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
	CLIP_MIDDLELEFT,
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
	void OnEvent(const Event& new_event);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	// Utility ---------------------

	float3 GetCursorPosFromLetter(const int& pos);

	UI_Label* GetLabel() const;

	ClipTextType GetClipping() const; 
	void SetClipping(const ClipTextType new_clip); 
	void TranslateEnclosedPlane(float3 increment);
	void UpdateContainerPlane();
	void RenderContainerPlane(); 
	float3 GetContainerPlaneCenter();
	float2 GetContainerPlaneSize();
	float3 GetClippingDistance(const ClipTextType new_clip);

	// Wrapping --------------------

	float2 GetSection() const;
	void SetSection(const float2 new_section); 

	void MoveSectionForward();
	void MovesectionBackward();

	horizontalTextOverflow GetHorizontalOverflow() const;
	void SetHorizontalOverflow(const horizontalTextOverflow new_ov);

	verticalTextOverflow GetVerticalOverflow() const;
	void SetVerticalOverflow(const verticalTextOverflow new_ov);

	UID GetParentTextInputUID() const;
	void SetParentTextInputUID(const UID& new_ov);

	float3 container_plane_vertices[4];
	int line_spacing = 0;

private:

	horizontalTextOverflow horizontal_overflow = HORIZONTAL_OVERFLOW;	// Horizontal text wrapping
	verticalTextOverflow vertical_overflow = VERTICAL_OVERFLOW;			// vertical text wrapping	
	
	UID text_input_ID = 0; 

	float2 text_origin = {0,0};
	float2 section = {0,0};														// Section of the text that will be rendered (normally used for text input)
	UI_Label * label = nullptr;
	float3 draw_color = {0,0,0};
	ClipTextType clipping = ClipTextType::CLIP_BOTTOMLEFT; 
};

