#pragma once

class Texture; 

class Material
{
public:
	Material();
	~Material();

	void CleanUp(); 

	Texture* GetDiffuseTexture(); 
	void SetDiffuseTexture(Texture* new_tex); 

private:

	Texture * diffuse; 
};

