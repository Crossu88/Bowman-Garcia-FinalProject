#include "Solarbody.h"

Solarbody::Solarbody(Solarbody *parent, float radius, float orbitTime, glm::vec3 offset, const char *diffusePath, const char *normalPath)
{
    if (parent != nullptr)
        m_parent = parent;

    m_radius = radius;
    m_orbitTime = orbitTime;
    m_orbitOffset = offset;

    m_diffuse = new Texture(diffusePath);
    m_normal = new Texture(normalPath);
}

Solarbody::Solarbody(const char* diffusePath, const char* normalPath) : Solarbody(nullptr, 1.0f, 10.0f, glm::vec3(1.0f), diffusePath, normalPath) { }

Solarbody::~Solarbody()
{
}

void Solarbody::Update()
{
}

void Solarbody::Render()
{
}
