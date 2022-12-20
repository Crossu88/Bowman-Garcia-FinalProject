#ifndef SOLARBODY_H
#define SOLARBODY_H

#include "graphics_headers.h"
#include "sphere.h"
#include "texture.h"
#include "Material.h"
#include "ShaderFile.h"

class Solarbody
{
    public:
        Solarbody(const char* diffusePath, const char* normalPath, int precision = 48);
        ~Solarbody();

        void SetOrbitParameters(float distance, float time, glm::vec3 offset);
        void SetBodyParameters(float radius, float spinSpeed);

        Material* GetMaterial() { return m_body->GetMaterial(); }

        void Update(double time, glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
        void Render();

        void AddChild(Solarbody* child);

    private:
        std::vector<Solarbody*> m_children;
        Sphere* m_body;

        float m_radius;
        float m_speed;

        float m_orbitInitial;
        float m_orbitDistance;
        float m_orbitTime;
        glm::vec3 m_orbitOffset;

        glm::vec3 CalculateOrbitPos(double time);

};

#endif /* SOLARBODY_H */