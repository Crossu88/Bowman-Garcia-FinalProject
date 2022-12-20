#include "Material.h"

// Static members
ShaderFile* Material::m_shader = nullptr;
Camera* Material::m_camera = nullptr;


// Public

Material::Material(const char* diffusePath, const char* normalPath, unsigned int texFlags, float metallic, float roughness, float ao)
{
    m_diffuseTexture = new Texture(diffusePath, texFlags);
    m_normalTexture = new Texture(normalPath, texFlags);

    m_metallic = metallic;
    m_roughness = roughness;
    m_occlusion = ao;
}
Material::Material(const char* diffusePath, const char* normalPath, unsigned int texFlags) : Material(diffusePath, normalPath, texFlags, 1.0f, 1.0f, 1.0f) {}


Material::~Material()
{
    delete m_diffuseTexture;
    delete m_normalTexture;
    delete m_shader;
}

void Material::SetMaterialProperties(float metallic, float roughness, float occlusion)
{
    m_metallic = metallic;
    m_roughness = roughness;
    m_occlusion = occlusion;
}

void Material::Render(glm::mat4 model)
{
    // glm::mat4 transformation; // your transformation matrix.
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, translation, skew, perspective);

    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(12000.0f, 10000.0f, 6000.0f),
    };

    // Shader setup
    m_shader->use();
    m_shader->setMat4("view", m_camera->GetView());
    m_shader->setVec3("camPos", m_camera->GetPosition());
    m_shader->setMat4("model", model);
    m_shader->setFloat("metallic", m_metallic);
    m_shader->setFloat("roughness", m_roughness);
    m_shader->setFloat("ao", m_occlusion);

    // Bind material textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->getTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture->getTextureID());

    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        // glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        // newPos = lightPositions[i];
        glm::vec3 newPos = -translation;
        m_shader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        m_shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
}

// Private

void Material::InitializeShader(Camera* camera)
{
    if (m_shader != nullptr) return;

    m_camera = camera;

    m_shader = new ShaderFile("shaders\\hybridpbr.vs", "shaders\\hybridpbr.fs");
	m_shader->use();
	m_shader->setInt("albedoMap", 0);
	m_shader->setInt("normalMap", 1);
    m_shader->setMat4("projection", m_camera->GetProjection());
}