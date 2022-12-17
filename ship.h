#ifndef SHIP_H
#define SHIP_H

#include "graphics_headers.h"
#include "camera.h"
#include "mesh.h"

class Ship
{
    public:
        // Constructors
        Ship();
        Ship(Mesh* mesh, Camera* camera);

        // Destructor
        ~Ship();

        void Update(double deltaTime);

    private:
        // Transform data
        glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        // Movement data
        float m_maxSpeed = 20.0f;
        float m_currentSpeed = 0.0f;
        float m_acceleration = 10.0f;

        glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        // References
        Mesh* m_mesh;
        Camera* m_camera;

};

#endif /* SHIP_H */