#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Color.h"
#include "Resource.h"

class Texture; 

class Material : public Resource
{
public:
	Material();
	~Material();

	void CleanUp(); 

	Texture* GetDiffuseTexture(); 
	void SetDiffuseTexture(Texture* new_tex); 

	bool IsLoadedToMemory(); 

	void LoadToMemory(); 
	void UnloadFromMemory();

	void CreateMeta(); 

	void SetColor(Color color); 

	Texture * diffuse;

	Color color;

private:


};

#endif