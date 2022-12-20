#include "Texture.h"

// Constructs a texture and loads a file
Texture::Texture(const char* fileName, unsigned int flags) 
{
	loadTexture(fileName, flags);
}

Texture::Texture(const char* fileName) : Texture(fileName, SOIL_FLAG_MIPMAPS) { }

// Constructs a texture object without loading a file
Texture::Texture() {
	m_TextureID = -1;
	printf("No Texture Data Provided.");
}

// Loads and initializes a texture file
bool Texture::loadTexture(const char* texFile, unsigned int flags = SOIL_FLAG_MIPMAPS) {

	// Loads a texture through SOIL 2
	m_TextureID = SOIL_load_OGL_texture(
		texFile, 
		SOIL_LOAD_AUTO, 
		SOIL_CREATE_NEW_ID, 
		flags
		// SOIL_FLAG_MIPMAPS
		//SOIL_FLAG_INVERT_Y
		);

	// If the texture file fails to load, print an error
	if (!m_TextureID) {
		printf("Failed: Could not open texture file: %s!\n", texFile);
		return false;
	}

	// If the texture image fails to initialize, print an error
	if (!initializeTexture()) {
		printf("Failed: Could not initialize texture!");
		return false;
	}

	return true;
}

// Initializes texture parameters
bool Texture::initializeTexture() {
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	
	return true;
}



