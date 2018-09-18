#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	void SetTextureID();
	unsigned int GetTextureID() const;

	void SetWidth();
	unsigned int GetWidth() const;

	void SetHeight();
	unsigned int GetHeight() const;

	void LoadToMemory();
	void UnloadFromMemory(); 
	void Clear(); 

private:
	unsigned int texture_id;
	unsigned int width;
	unsigned int height;
};

