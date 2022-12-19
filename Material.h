#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics_headers.h"
#include "texture.h"
#include "ShaderFile.h"

class Material
{
    public:
        Material(ShaderFile* shader, const char* diffusePath, const char* normalPath, float metallic, float roughness, float ao);
        ~Material();

    private:
        Texture* m_diffuseTexture;
        Texture* m_normalTexture;
        ShaderFile* m_shader;

        float m_metallic;
        float m_roughness;
        float m_occlusion;

};

#endif /* MATERIAL_H */