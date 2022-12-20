#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics_headers.h"
#include "camera.h"
#include "texture.h"
#include "ShaderFile.h"

class Material
{
    public:
        Material(const char* diffusePath, const char* normalPath, unsigned int texFlags, float metallic, float roughness, float ao);
        Material(const char* diffusePath, const char* normalPath, unsigned int texFlags);
        ~Material();

        static void InitializeShader(Camera* camera);
        static ShaderFile* GetShader() { return m_shader; };

        // Modifiers
        void SetMaterialProperties(float metallic, float roughness, float occlusion);

        // Accessors

        // Operations
        void Render(glm::mat4 model);

    private:
        static ShaderFile* m_shader;
        static Camera* m_camera;

        Texture* m_diffuseTexture;
        Texture* m_normalTexture;

        float m_metallic;
        float m_roughness;
        float m_occlusion;
};

#endif /* MATERIAL_H */