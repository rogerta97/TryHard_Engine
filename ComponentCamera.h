#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Timer.h"
#include "Camera.h"

class TextureMSAA;

struct CameraInterpolation
{
	Timer interpolation_timer;
	uint interpolation_ms;

	//Position
	LineSegment line;

	//Rotation
	float3 source_vec;
	float3 dst_vec;

	float3 center;

	bool interpolate;

	void Init()
	{
		interpolation_ms = 350.0f;
		source_vec = { 0,0,0 };
		dst_vec = { 0,0,0 };
		interpolate = false;
	}
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 
	void Draw(bool is_editor);

	//Movement
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	float4x4 GetRawViewMatrix() const;
	float* GetViewOpenGLViewMatrix();

	//Interpolation
	void FillInterpolationSegmentAndRot();
	bool InterpolateCamera(float time);
	float3 GetCamPointFromDistance(float3 center, float distance) const;

	//Save & Load
	void Load(JSON_Object* root_obj); 
	void Save(JSON_Object* root_obj, const char* root);

	//Setters & Getters
	void SetSpeed(float new_speed);
	float GetSpeed() const;

	void SetWidth();
	float GetWidth() const;
	void SetHeight();
	float GetHeight() const;
	Frustum* GetFrustum() const; 
	
	void SetMouseSensitivity(float new_sensitivity);
	float GetMouseSensitivity() const;

	void SetViewportTextureSize(uint x, uint y);
	void SetViewportTexture(TextureMSAA* new_tex); 

	void UpdateFrustumPositionAndRotation();

	//Utility
	void LockCamera();
	void UnlockCamera();
	bool IsLocked() const;
	void DrawFrustum(); 

	void PrintPreview();

	void CalculateViewMatrix();
	void SetEditorCamera();

	//Docking Texture
	TextureMSAA* GetViewportTexture();

	bool AreSame(float a, float b, float epsilon);

	bool AreFloat3Same(float3 a, float3 b, float epsilon);

public: 

	float3					X, Y, Z, Position, Reference;
	CameraInterpolation		interpolation; 
	float					wheel_zoom_speed = 5.0f;
	float					speed_multiplier;
	float					mouse_sensitivity;
	bool					draw_frustum; 
	Camera*					camera;
	bool					is_editor; 
	bool					is_rendering; 
	bool					center_next_frame; 

private:

	float4x4				ViewMatrix, ViewMatrixInverse; //It should follow gameobject transform and not having another ViewMatrix	 
	float2					size;

	bool					show_preview_panel;

	TextureMSAA*			viewport_texture;
	float					speed = 0.1f;

	bool					locked;
	bool					orbit = true;
};

#endif

