#ifndef SOLARBODY_H
#define SOLARBODY_H

#include "graphics_headers.h"
#include "sphere.h"
#include "texture.h"
#include "ShaderFile.h"

class Solarbody
{
    public:
        Solarbody(Solarbody* parent, float radius, float orbitTime, glm::vec3 offset, const char* diffusePath, const char* normalPath);
        Solarbody(const char* diffusePath, const char* normalPath);
        ~Solarbody();

        void Update();
        void Render();

    private:
        Solarbody* m_parent;
        Sphere* m_body;

        float m_radius;
        float m_orbitTime;
        glm::vec3 m_orbitOffset;

        Texture* m_diffuse;
        Texture* m_normal;

};

#endif /* SOLARBODY_H */