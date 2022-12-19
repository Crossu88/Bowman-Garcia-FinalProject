#include "Material.h"

// Public

Material::Material(ShaderFile* shader, const char* diffusePath, const char* normalPath, float metallic = 1.0f, float roughness = 1.0f, float ao = 1.0f)
{
    m_shader = shader;
    m_diffuseTexture = new Texture(diffusePath);
    m_normalTexture = new Texture(normalPath);
    m_metallic = metallic;
    m_roughness = roughness;
    m_occlusion = ao;
}

Material::~Material()
{
    delete m_diffuseTexture;
    delete m_normalTexture;
    delete m_shader;
}