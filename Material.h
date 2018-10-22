#pragma once

class Texture; 

class Material
{
public:
	Material();
	~Material();

	Texture* GetDiffuseTexture(); 
	void SetDiffuseTexture(Texture* new_tex); 

private:

	Texture * diffuse; 
};

