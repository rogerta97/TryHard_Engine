#pragma once

#include "Transform.h"
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	//Setters & Getters
	float3 GetPosition() const;
	Quat GetRotation() const; 
	float3 GetScale() const;

	float4x4 GetViewMatrix(); 

	float3 GetRotationEuler() const; 
	void SetRotationEuler(float3 new_rot);

	void SetPosition(float3 new_pos);
	void SetRotation(Quat new_pos);
	void SetScale(float3 new_pos);

private: 

	void CalculateViewMatrix();

private:

	Transform transform; 
	float4x4 ViewMatrix; 
};

