#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "Input.h"

class Camera
{
  public:
    Camera();
    ~Camera();

    bool Initialize(int w, int h);

    void Update(double dt);

    void SetSpeed(glm::vec3 newSpeed);


    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 m_speed;

    // Movement related variables
    glm::vec3 viewPos = glm::vec3(0.0f, 12.0f, 20.0f);
    glm::vec3 viewFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
    double yaw = -90.0;
    double pitch = -35.0;

    // Mouse related variables
    double lastX;
    double lastY;
    bool mouseNotSet = true;

    // Input processing
    void ProcessInput();
    void ProcessMouseInput(Input* input);
    void ProcessKeyboardInput(Input* input);

    // Helpers
    void CalcLookDirection();
};

#endif /* CAMERA_H */
