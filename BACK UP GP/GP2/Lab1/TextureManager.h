#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void BindTexture(unsigned int obj); // bind upto 32 textures
	void TextureLoader(const std::string& filename);
	GLuint getID() { return textureManager; }

private:

	GLuint textureManager;
	unsigned int textureID;
};

