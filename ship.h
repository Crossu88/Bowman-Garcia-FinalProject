#ifndef SHIP_H
#define SHIP_H

#include "graphics_headers.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"

class Ship
{
    public:
        // Constructors
        Ship();
        Ship(Mesh* mesh, Camera* camera);

        // Destructor
        ~Ship();

        // Public functions
        void Update(double deltaTime);

    private:
        // Transform data
        Transform* m_shipTransform;
        Transform* m_meshTransform;
        Transform* m_cameraTransform;

        // Movement data
        float m_maxSpeed = 20.0f;
        float m_currentSpeed = 0.0f;
        float m_acceleration = 10.0f;

        glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

        // References
        Mesh* m_mesh;
        Camera* m_camera;

        // Mouse related variables
        double lastX;
        double lastY;
        bool mouseNotSet = true;

        // Private functions
        void UpdatePhysics(double deltaTime);
        void UpdateMesh();
        void UpdateCamera();

        // Input
        void ProcessInput();
        void ProcessKeyboardInput(Input* input);
        void ProcessMouseInput(Input* input);

};

#endif /* SHIP_H */