#include "Solarbody.h"

Solarbody::Solarbody(const char *diffusePath, const char *normalPath, int precision)
{
    m_body = new Sphere(precision, diffusePath, normalPath);

    m_orbitInitial = glm::linearRand(0.0f, 1.0f);
}
Solarbody::~Solarbody() { }

void Solarbody::SetOrbitParameters(float distance, float time, glm::vec3 offset)
{
    m_orbitDistance = distance;
    m_orbitTime = time;
    m_orbitOffset = offset;
}

void Solarbody::AddChild(Solarbody* child)
{
    m_children.push_back(child);
}

void Solarbody::SetBodyParameters(float radius, float speed)
{
    m_radius = radius;
    m_speed = speed;
}

void Solarbody::Update(double time, glm::mat4 origin)
{
    float fTime = (float) time;
    float twoPi = 2.0 * glm::pi<float>();

    glm::mat4 transform = origin * glm::translate(glm::mat4(1.0f), m_orbitDistance * CalculateOrbitPos(time));

    if (m_children.size() > 0)
        for(Solarbody* child : m_children)
            child->Update(time, transform);

    transform *= glm::rotate(glm::mat4(1.0f), fTime * (twoPi / m_speed), glm::vec3(0.0f, 1.0f, 0.0f));
    transform *= glm::scale(glm::vec3(m_radius));

    m_body->SetModel(transform);
}

void Solarbody::Render()
{
    m_body->Render();

    if (m_children.size() > 0)
        for(Solarbody* child : m_children)
            child->Render();
}

// Private

glm::vec3 Solarbody::CalculateOrbitPos(double time)
{
    double twoPi = 2.0 * glm::pi<double>();
    double period = twoPi * ( (time / m_orbitTime) + m_orbitInitial );
    // double period = sin(glm::radians(2.0 * glm::pi<double>() * (time / m_orbitTime)));
    // double period = sin(2.0 * glm::pi<double>() * (time / m_orbitTime));
    return glm::vec3(cos(period) * m_orbitOffset.x, sin(period) * m_orbitOffset.y, sin(period) * m_orbitOffset.z);
}
