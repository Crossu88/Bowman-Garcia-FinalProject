#pragma once
#include "graphics_headers.h"
#include <SOIL2/SOIL2.h>

class Texture
{

public:
	Texture(const char* fileName, unsigned int flags);
	Texture(const char* fileName);
	Texture();
	bool loadTexture(const char* texFile, unsigned int flags);
	GLuint getTextureID() { return m_TextureID; }

private:
	GLuint m_TextureID;

	bool initializeTexture();

};

