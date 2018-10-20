#pragma once

#include "Transform.h"
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	void ResetTransform(); 

	//Setters & Getters
	float3 GetPosition() const;
	Quat GetRotation() const; 
	float3 GetScale() const;

	float4x4 GetViewMatrix(); 
	float4x4 GetGlobalViewMatrix(); 

	float3 GetRotationEuler() const; 
	void SetRotationEuler(float3 new_rot, const char* axis);

	void SetPosition(float3 new_pos);
	void SetRotation(Quat new_pos);
	void SetScale(float3 new_pos);

	Transform transform;
	void DrawAxis();

private: 

	void CalculateViewMatrix();

private: 
	float4x4 ViewMatrix;
	bool dirty; 
};

