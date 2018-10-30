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

	void SetViewMatrix(float4x4 new_mat);

	float3 GetRotationEuler() const; 
	void SetRotationEuler(float3 new_rot);

	void SetPosition(float3 new_pos);
	void SetRotation(Quat new_pos);
	void SetScale(float3 new_pos);

	Transform transform;
	void DrawAxis();

	bool HasTransformed();
	void SetHasTransformed(bool value);
	void CalculateGlobalViewMatrix();

	void CalculateViewMatrix();

private: 
	bool has_transformed;

	

private: 
	float4x4 ViewMatrix;
	float4x4 GlobalMatrix;
	bool dirty; 
};

